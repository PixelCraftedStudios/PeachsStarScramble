// bobomb.inc.c

// -----------------------------
// QoL constants
// -----------------------------

#define BOBOMB_FUSE_TIME              150
#define BOBOMB_FUSE_FAST_THRESHOLD    120

#define BOBOMB_PATROL_SPEED           5.0f
#define BOBOMB_CHASE_SPEED            20.0f
#define BOBOMB_THROW_FORWARD_VEL      25.0f
#define BOBOMB_THROW_UPWARD_VEL       20.0f
#define BOBOMB_KNOCKBACK_FORWARD_VEL  25.0f
#define BOBOMB_KNOCKBACK_UPWARD_VEL   30.0f

#define BOBOMB_HOME_RADIUS            400
#define BOBOMB_CHASE_ANGLE_RANGE      0x2000
#define BOBOMB_CHASE_TURN_RATE        0x0800
#define BOBOMB_CHASE_LOST_DISTANCE    2000.0f

#define BOBOMB_CLIFF_DROP_THRESHOLD   -80.0f

#define BOBOMB_SOFT_LAND_VEL_Y        -10.0f
#define BOBOMB_SOFT_LAND_FWD_VEL      10.0f

static struct ObjectHitbox sBobombHitbox = {
    /* interactType:      */ INTERACT_GRABBABLE,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 0,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 65,
    /* height:            */ 113,
    /* hurtboxRadius:     */ 0,
    /* hurtboxHeight:     */ 0,
};

// -----------------------------
// Core init
// -----------------------------

void bhv_bobomb_init(void) {
    o->oGravity = 2.5f;
    o->oFriction = 0.8f;
    o->oBuoyancy = 1.3f;
    o->oInteractionSubtype = INT_SUBTYPE_KICKABLE;

    // QoL: ensure fuse state is clean on spawn
    o->oBobombFuseLit = FALSE;
    o->oBobombFuseTimer = 0;
}

// -----------------------------
// Helpers
// -----------------------------

static void bobomb_update_fuse(void) {
    s8 dustPeriodMinus1;

    if (!o->oBobombFuseLit) {
        return;
    }

    if (o->oBobombFuseTimer > BOBOMB_FUSE_FAST_THRESHOLD) {
        dustPeriodMinus1 = 1;
    } else {
        dustPeriodMinus1 = 7;
    }
    
    // QoL: Flash when close to exploding
    if (o->oBobombFuseTimer > BOBOMB_FUSE_TIME - 40) {
        if ((o->oBobombFuseTimer & 1) == 0) {
            o->header.gfx.node.flags |= GRAPH_RENDER_INVISIBLE;
        } else {
            o->header.gfx.node.flags &= ~GRAPH_RENDER_INVISIBLE;
        }
    }



    // oBobombFuseTimer % 2 or oBobombFuseTimer % 8
    if (!(dustPeriodMinus1 & o->oBobombFuseTimer)) {
        spawn_object(o, MODEL_SMOKE, bhvBobombFuseSmoke);
    }

    cur_obj_play_sound_1(SOUND_AIR_BOBOMB_LIT_FUSE);
    o->oBobombFuseTimer++;
}

static void bobomb_start_chase(void) {
    o->oBobombFuseLit = TRUE;
    o->oBobombFuseTimer = 0;
    o->oAction = BOBOMB_ACT_CHASE_MARIO;
}

static void bobomb_reset_to_patrol(void) {
    o->oHeldState = HELD_FREE;
    o->oAction = BOBOMB_ACT_PATROL;
    o->oForwardVel = BOBOMB_PATROL_SPEED;
}

static void bobomb_handle_floor_safety(s16 collisionFlags) {
    obj_check_floor_death(collisionFlags, sObjFloor);

    // QoL: avoid walking off cliffs by turning around when floor drops sharply
    if (o->oFloorHeight - o->oPosY < BOBOMB_CLIFF_DROP_THRESHOLD) {
        o->oMoveAngleYaw += 0x4000;
    }
}

// -----------------------------
// Coin spawn
// -----------------------------

void bobomb_spawn_coin(void) {
    if (!(GET_BPARAM3(o->oBehParams) & RESPAWN_INFO_TYPE_NORMAL)) {
        obj_spawn_yellow_coins(o, 1);
        SET_FULL_BPARAM3(o->oBehParams, RESPAWN_INFO_TYPE_NORMAL);
        set_object_respawn_info_bits(o, RESPAWN_INFO_TYPE_NORMAL);
    }
}

// -----------------------------
// Explosion
// -----------------------------

void bobomb_act_explode(void) {
    if (o->oTimer == 0) {
        struct Object *bobombStar = cur_obj_nearest_object_with_behavior(bhvBobombStar);

        if (bobombStar != NULL) {
            bobombStar->oHiddenStarTriggerCounter++;
        }
    }

    if (o->oTimer < 5) {
        cur_obj_scale(1.0f + ((f32) o->oTimer / 5.0f));
    } else {
        struct Object *explosion = spawn_object(o, MODEL_EXPLOSION, bhvExplosion);
        explosion->oGraphYOffset += 100.0f;

        bobomb_spawn_coin();
        create_respawner(MODEL_BLACK_BOBOMB, bhvBobomb, 3000);

        o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
    }
}

// -----------------------------
// Interactions
// -----------------------------

void bobomb_check_interactions(void) {
    obj_set_hitbox(o, &sBobombHitbox);

    if (o->oInteractStatus & INT_STATUS_INTERACTED) {
        if (o->oInteractStatus & INT_STATUS_MARIO_KNOCKBACK_DMG) {
            o->oMoveAngleYaw = gMarioObject->header.gfx.angle[1];
            o->oForwardVel = BOBOMB_KNOCKBACK_FORWARD_VEL;
            o->oVelY = BOBOMB_KNOCKBACK_UPWARD_VEL;
            o->oAction = BOBOMB_ACT_LAUNCHED;
        }

        if (o->oInteractStatus & INT_STATUS_TOUCHED_BOB_OMB) {
            o->oAction = BOBOMB_ACT_EXPLODE;
        }

        o->oInteractStatus = INT_STATUS_NONE;
    }

    if (obj_attack_collided_from_other_object(o)) {
        o->oAction = BOBOMB_ACT_EXPLODE;
    }
}

// -----------------------------
// Movement states
// -----------------------------

void bobomb_act_patrol(void) {
    o->oForwardVel = BOBOMB_PATROL_SPEED;

    s16 collisionFlags = object_step();

    // QoL: floor safety (cliff avoidance + death)
    bobomb_handle_floor_safety(collisionFlags);

    if (obj_return_home_if_safe(o, o->oHomeX, o->oHomeY, o->oHomeZ, BOBOMB_HOME_RADIUS)
     && obj_check_if_facing_toward_angle(o->oMoveAngleYaw, o->oAngleToMario, BOBOMB_CHASE_ANGLE_RANGE)) {
        bobomb_start_chase();
    }
}

void bobomb_act_chase_mario(void) {
    s16 animFrame = ++o->header.gfx.animInfo.animFrame;

    o->oForwardVel = BOBOMB_CHASE_SPEED;
    s16 collisionFlags = object_step();

    if (animFrame == 5 || animFrame == 16) {
        cur_obj_play_sound_2(SOUND_OBJ_BOBOMB_WALK);
    }

    obj_turn_toward_object(o, gMarioObject, O_MOVE_ANGLE_YAW_INDEX, BOBOMB_CHASE_TURN_RATE);
    bobomb_handle_floor_safety(collisionFlags);

    // QoL: stop chasing if Mario is too far away
    if (o->oDistanceToMario > BOBOMB_CHASE_LOST_DISTANCE) {
        bobomb_reset_to_patrol();
    }
}

void bobomb_act_launched(void) {
    s16 collisionFlags = object_step();

    if ((collisionFlags & OBJ_COL_FLAG_GROUNDED) == OBJ_COL_FLAG_GROUNDED) {
        // QoL: only explode on meaningful impact; soft landings return to patrol
        if (o->oVelY < BOBOMB_SOFT_LAND_VEL_Y || o->oForwardVel > BOBOMB_SOFT_LAND_FWD_VEL) {
            o->oAction = BOBOMB_ACT_EXPLODE;
        } else {
            bobomb_reset_to_patrol();
        }
    }
}

// -----------------------------
// Free loops (generic / stationary)
// -----------------------------

static void bobomb_free_common(void) {
    switch (o->oAction) {
        case BOBOMB_ACT_PATROL:
            bobomb_act_patrol();
            break;

        case BOBOMB_ACT_LAUNCHED:
            bobomb_act_launched();
            break;

        case BOBOMB_ACT_CHASE_MARIO:
            bobomb_act_chase_mario();
            break;

        case BOBOMB_ACT_EXPLODE:
            bobomb_act_explode();
            break;

        case OBJ_ACT_LAVA_DEATH:
            if (obj_lava_death()) {
                create_respawner(MODEL_BLACK_BOBOMB, bhvBobomb, 3000);
            }
            break;

        case OBJ_ACT_DEATH_PLANE_DEATH:
            o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
            create_respawner(MODEL_BLACK_BOBOMB, bhvBobomb, 3000);
            break;
    }

    bobomb_check_interactions();

    if (o->oBobombFuseTimer > BOBOMB_FUSE_TIME) {
        o->oAction = BOBOMB_ACT_EXPLODE;
    }
}

void generic_bobomb_free_loop(void) {
    bobomb_free_common();
}

void stationary_bobomb_free_loop(void) {
    // Stationary variant skips patrol/chase; only reacts when launched or forced to explode
    switch (o->oAction) {
        case BOBOMB_ACT_LAUNCHED:
            bobomb_act_launched();
            break;

        case BOBOMB_ACT_EXPLODE:
            bobomb_act_explode();
            break;

        case OBJ_ACT_LAVA_DEATH:
            if (obj_lava_death()) {
                create_respawner(MODEL_BLACK_BOBOMB, bhvBobomb, 3000);
            }
            break;

        case OBJ_ACT_DEATH_PLANE_DEATH:
            o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
            create_respawner(MODEL_BLACK_BOBOMB, bhvBobomb, 3000);
            break;
    }

    bobomb_check_interactions();

    if (o->oBobombFuseTimer > BOBOMB_FUSE_TIME) {
        o->oAction = BOBOMB_ACT_EXPLODE;
    }
}

void bobomb_free_loop(void) {
    if (o->oBehParams2ndByte == BOBOMB_BP_STYPE_GENERIC) {
        generic_bobomb_free_loop();
    } else {
        stationary_bobomb_free_loop();
    }
}

// -----------------------------
// Held / dropped / thrown
// -----------------------------

void bobomb_held_loop(void) {
    o->header.gfx.node.flags |= GRAPH_RENDER_INVISIBLE;
    cur_obj_init_animation(BOBOMB_ANIM_HELD);
    cur_obj_set_pos_relative(gMarioObject, 0.0f, 60.0f, 100.0f);

    o->oBobombFuseLit = TRUE;

    if (o->oBobombFuseTimer > BOBOMB_FUSE_TIME) {
        // QoL: force Mario to drop and immediately transition to explode
        gMarioObject->oInteractStatus |= INT_STATUS_MARIO_DROP_OBJECT;
        o->oAction = BOBOMB_ACT_EXPLODE;
    }
}

void bobomb_dropped_loop(void) {
    cur_obj_get_dropped();

    o->header.gfx.node.flags &= ~GRAPH_RENDER_INVISIBLE;
    cur_obj_init_animation(BOBOMB_ANIM_WALKING);

    bobomb_reset_to_patrol();
}

void bobomb_thrown_loop(void) {
    cur_obj_enable_rendering();

    o->header.gfx.node.flags &= ~GRAPH_RENDER_INVISIBLE;
    o->oHeldState = HELD_FREE;
    o->oFlags &= ~OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW;
    o->oForwardVel = BOBOMB_THROW_FORWARD_VEL;
    o->oVelY = BOBOMB_THROW_UPWARD_VEL;
    o->oAction = BOBOMB_ACT_LAUNCHED;
}

// -----------------------------
// Blink helper
// -----------------------------

void curr_obj_random_blink(s32 *blinkTimer) {
    if (*blinkTimer == 0) {
        if ((s16)(random_float() * 100.0f) == 0) {
            o->oAnimState = 1;
            *blinkTimer = 1;
        }
    } else {
        (*blinkTimer)++;

        if (*blinkTimer > 5) {
            o->oAnimState = 0;
        }

        if (*blinkTimer > 10) {
            o->oAnimState = 1;
        }

        if (*blinkTimer > 15) {
            o->oAnimState = 0;
            *blinkTimer = 0;
        }
    }
}

// -----------------------------
// Main Bob-omb loop
// -----------------------------

void bhv_bobomb_loop(void) {
    if (is_point_within_radius_of_mario(o->oPosX, o->oPosY, o->oPosZ, 4000)) {
        switch (o->oHeldState) {
            case HELD_FREE:
                bobomb_free_loop();
                break;

            case HELD_HELD:
                bobomb_held_loop();
                break;

            case HELD_THROWN:
                bobomb_thrown_loop();
                break;

            case HELD_DROPPED:
                bobomb_dropped_loop();
                break;
        }

        curr_obj_random_blink(&o->oBobombBlinkTimer);
        bobomb_update_fuse();
    }
}

// -----------------------------
// Fuse smoke
// -----------------------------

void bhv_bobomb_fuse_smoke_init(void) {
    o->oPosX += (s32)(random_float() * 80.0f) - 40;
    o->oPosY += (s32)(random_float() * 80.0f) + 60;
    o->oPosZ += (s32)(random_float() * 80.0f) - 40;
    cur_obj_scale(1.2f);
}

// -----------------------------
// Bob-omb Buddy
// -----------------------------

void bhv_bobomb_buddy_init(void) {
    o->oGravity = 2.5f;
    o->oFriction = 0.8f;
    o->oBuoyancy = 1.3f;
    o->oInteractionSubtype = INT_SUBTYPE_NPC;
}

void bobomb_buddy_act_idle(void) {
    s16 animFrame = o->header.gfx.animInfo.animFrame;

    object_step();

    if (animFrame == 5 || animFrame == 16) {
        cur_obj_play_sound_2(SOUND_OBJ_BOBOMB_WALK);
    }

    if (o->oDistanceToMario < 1000.0f) {
        o->oMoveAngleYaw = approach_s16_symmetric(o->oMoveAngleYaw, o->oAngleToMario, 0x140);
    }

    if (o->oInteractStatus == INT_STATUS_INTERACTED) {
        o->oAction = BOBOMB_BUDDY_ACT_TURN_TO_TALK;
    }
}

/**
 * Function for the Bob-omb Buddy cannon guy.
 * dialogFirstText is the first dialogID called when Bob-omb Buddy
 * starts to talk to Mario to prepare the cannon(s) for him.
 * Then the camera goes to the nearest cannon, to play the "prepare cannon" cutscene
 * dialogSecondText is called after Bob-omb Buddy has the cannon(s) ready and
 * then tells Mario that is "Ready for blastoff".
 */
void bobomb_buddy_cannon_dialog(s16 dialogFirstText, s16 dialogSecondText) {
    struct Object *cannonClosed;
    s16 buddyText, cutscene;

    switch (o->oBobombBuddyCannonStatus) {
        case BOBOMB_BUDDY_CANNON_UNOPENED:
            buddyText = cutscene_object_with_dialog(CUTSCENE_DIALOG, o, dialogFirstText);
            if (buddyText != DIALOG_RESPONSE_NONE) {
                save_file_set_cannon_unlocked();
                cannonClosed = cur_obj_nearest_object_with_behavior(bhvCannonClosed);
                if (cannonClosed != NULL) {
                    o->oBobombBuddyCannonStatus = BOBOMB_BUDDY_CANNON_OPENING;
                } else {
                    o->oBobombBuddyCannonStatus = BOBOMB_BUDDY_CANNON_STOP_TALKING;
                }
            }
            break;

        case BOBOMB_BUDDY_CANNON_OPENING:
            cannonClosed = cur_obj_nearest_object_with_behavior(bhvCannonClosed);
            cutscene = cutscene_object(CUTSCENE_PREPARE_CANNON, cannonClosed);
            if (cutscene == -1) {
                o->oBobombBuddyCannonStatus = BOBOMB_BUDDY_CANNON_OPENED;
            }
            break;

        case BOBOMB_BUDDY_CANNON_OPENED:
            buddyText = cutscene_object_with_dialog(CUTSCENE_DIALOG, o, dialogSecondText);
            if (buddyText != DIALOG_RESPONSE_NONE) {
                o->oBobombBuddyCannonStatus = BOBOMB_BUDDY_CANNON_STOP_TALKING;
            }
            break;

        case BOBOMB_BUDDY_CANNON_STOP_TALKING:
            set_mario_npc_dialog(MARIO_DIALOG_STOP);

            o->activeFlags &= ~ACTIVE_FLAG_INITIATED_TIME_STOP;
            o->oBobombBuddyHasTalkedToMario = BOBOMB_BUDDY_HAS_TALKED;
            o->oInteractStatus = INT_STATUS_NONE;
            o->oAction = BOBOMB_BUDDY_ACT_IDLE;
            o->oBobombBuddyCannonStatus = BOBOMB_BUDDY_CANNON_OPENED;
            break;
    }
}

void bobomb_buddy_act_talk(void) {
    if (set_mario_npc_dialog(MARIO_DIALOG_LOOK_FRONT) == MARIO_DIALOG_STATUS_SPEAK) {
        o->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;

        switch (o->oBobombBuddyRole) {
            case BOBOMB_BUDDY_ROLE_ADVICE:
                if (cutscene_object_with_dialog(CUTSCENE_DIALOG, o, o->oBehParams2ndByte)
                    != BOBOMB_BUDDY_BP_STYPE_GENERIC) {
                    set_mario_npc_dialog(MARIO_DIALOG_STOP);

                    o->activeFlags &= ~ACTIVE_FLAG_INITIATED_TIME_STOP;
                    o->oBobombBuddyHasTalkedToMario = BOBOMB_BUDDY_HAS_TALKED;
                    o->oInteractStatus = INT_STATUS_NONE;
                    o->oAction = BOBOMB_BUDDY_ACT_IDLE;
                }
                break;

            case BOBOMB_BUDDY_ROLE_CANNON:
                if (gCurrCourseNum == COURSE_BOB) {
                    bobomb_buddy_cannon_dialog(DIALOG_004, DIALOG_105);
                } else {
                    bobomb_buddy_cannon_dialog(DIALOG_047, DIALOG_106);
                }
                break;
        }
    }
}

void bobomb_buddy_act_turn_to_talk(void) {
    s16 animFrame = o->header.gfx.animInfo.animFrame;

    if (animFrame == 5 || animFrame == 16) {
        cur_obj_play_sound_2(SOUND_OBJ_BOBOMB_WALK);
    }

    o->oMoveAngleYaw = approach_s16_symmetric(o->oMoveAngleYaw, o->oAngleToMario, 0x1000);

    if ((s16) o->oMoveAngleYaw == (s16) o->oAngleToMario) {
        o->oAction = BOBOMB_BUDDY_ACT_TALK;
    }

    cur_obj_play_sound_2(SOUND_ACTION_READ_SIGN);
}

void bobomb_buddy_actions(void) {
    switch (o->oAction) {
        case BOBOMB_BUDDY_ACT_IDLE:
            bobomb_buddy_act_idle();
            break;

        case BOBOMB_BUDDY_ACT_TURN_TO_TALK:
            bobomb_buddy_act_turn_to_talk();
            break;

        case BOBOMB_BUDDY_ACT_TALK:
            bobomb_buddy_act_talk();
            break;
    }

    set_object_visibility(o, 3000);
}

void bhv_bobomb_buddy_loop(void) {
    bobomb_buddy_actions();

    curr_obj_random_blink(&o->oBobombBuddyBlinkTimer);

    o->oInteractStatus = INT_STATUS_NONE;
}
