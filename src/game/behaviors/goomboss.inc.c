// goomboss.inc.c
// Goomboss: walks toward Mario (idle anim), attacks with throw anim when close
// (hurts Mario on a specific frame), takes 3 head stomps to die, then spawns a star.

// -------------------------------------------------------
// Tunables
// -------------------------------------------------------
#define GOOMBOSS_SCALE              6.0f
#define GOOMBOSS_MAX_HEALTH         3
#define GOOMBOSS_FACE_RATE          0x300
#define GOOMBOSS_MODEL_YAW_OFFSET  -0x4000
#define GOOMBOSS_FOLLOW_SPEED       8.0f
#define GOOMBOSS_ATTACK_DIST        140.0f   // switch to attack when Mario is very close
#define GOOMBOSS_ATTACK_HIT_DIST    350.0f   // range at which the swing connects
#define GOOMBOSS_ATTACK_HIT_FRAME   8        // throw-anim frame the fist lands
#define GOOMBOSS_ATTACK_COOLDOWN    90       // frames between attacks
#define GOOMBOSS_FIRST_ATTACK_DELAY 45       // initial chase time before first attack
#define GOOMBOSS_STOMP_COOLDOWN     60       // invuln frames after being stomped
#define GOOMBOSS_HIT_STUN_FRAMES    50       // squish-stun duration after a stomp
#define GOOMBOSS_STOMP_BOUNCE_VEL   36.0f    // Mario upward velocity after stomping head
#define GOOMBOSS_SQUISH_SCALE_Y     0.35f    // peak Y squish at start of stun

#define GOOMBOSS_ANIM_IDLE           0
#define GOOMBOSS_ANIM_THROW          1  // repurposed as melee attack animation
#define GOOMBOSS_ANIM_DIE            2

#define GOOMBOSS_ACT_INTRO           0
#define GOOMBOSS_ACT_ACTIVE          1
#define GOOMBOSS_ACT_HIT             2
#define GOOMBOSS_ACT_DEATH           3
#define GOOMBOSS_ACT_STOP_MUSIC      4
#define GOOMBOSS_ACT_ATTACK          5

#define GOOMBOSS_INTRO_DIALOG  DIALOG_155
#define GOOMBOSS_DEATH_DIALOG  DIALOG_156

// -------------------------------------------------------
// Hitbox
// damageOrCoinValue = 0: contact alone does NOT hurt Mario;
// the attack state deals damage explicitly.
// INTERACT_BOUNCE_TOP allows Mario to stomp the head.
// -------------------------------------------------------

static struct ObjectHitbox sGoombossHitbox = {
    /* interactType:      */ INTERACT_BOUNCE_TOP,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 0,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 200,
    /* height:            */ 250,
    /* hurtboxRadius:     */ 150,
    /* hurtboxHeight:     */ 220,
};

// -------------------------------------------------------
// Helpers
// -------------------------------------------------------

// Returns TRUE if Mario bounced off the head this frame.
static s32 goomboss_check_stomped(void) {
    return (o->oInteractStatus & INT_STATUS_WAS_ATTACKED) != 0;
}

// Give Mario a high triple-jump arc.
static void goomboss_bounce_mario_high(void) {
    set_mario_action(gMarioState, ACT_TRIPLE_JUMP, 1);
    gMarioState->vel[1] = GOOMBOSS_STOMP_BOUNCE_VEL;
}

// Deal one health wedge of damage to Mario if he isn't already invincible.
static void goomboss_hurt_mario(void) {
    if (gMarioState->invincTimer > 0) { return; }
    gMarioState->health -= 0x100;
    gMarioState->invincTimer = 30;
    if (gMarioState->action & ACT_FLAG_AIR) {
        set_mario_action(gMarioState, ACT_HARD_BACKWARD_AIR_KB, 0);
    } else {
        set_mario_action(gMarioState, ACT_HARD_BACKWARD_GROUND_KB, 0);
    }
    mario_set_forward_vel(gMarioState, -30.0f);
    gMarioState->vel[1] = 25.0f;
}

// -------------------------------------------------------
// Action: INTRO
// -------------------------------------------------------

static void goomboss_act_intro(void) {
    o->oForwardVel = 0.0f;

    if (o->oSubAction == 0) {
        cur_obj_become_intangible();
        cur_obj_set_pos_to_home();
        o->oHealth = GOOMBOSS_MAX_HEALTH;
        o->oNumLootCoins = GOOMBOSS_MAX_HEALTH; // dedicated remaining stomp counter
        o->oSubAction = 1;
        seq_player_lower_volume(SEQ_PLAYER_LEVEL, 60, 40);
    } else {
        if (cur_obj_update_dialog(MARIO_DIALOG_LOOK_UP,
                DIALOG_FLAG_TEXT_DEFAULT, GOOMBOSS_INTRO_DIALOG, 0)) {
            play_music(SEQ_PLAYER_LEVEL, SEQUENCE_ARGS(4, SEQ_EVENT_BOSS), 0);
            cur_obj_become_tangible();
            o->oGoombossHitCooldown = 0;
            o->oGoombossThrowTimer  = GOOMBOSS_FIRST_ATTACK_DELAY;
            o->oAction = GOOMBOSS_ACT_ACTIVE;
        }
    }
}

// -------------------------------------------------------
// Action: ACTIVE — walk toward Mario in idle animation
// -------------------------------------------------------

static void goomboss_act_active(void) {
    obj_set_hitbox(o, &sGoombossHitbox);
    cur_obj_rotate_yaw_toward(o->oAngleToMario + GOOMBOSS_MODEL_YAW_OFFSET, GOOMBOSS_FACE_RATE);

    // Stomp check
    if (o->oGoombossHitCooldown > 0) {
        o->oGoombossHitCooldown--;
    } else if (goomboss_check_stomped()) {
        o->oNumLootCoins--;
        o->oGoombossHitCooldown = GOOMBOSS_STOMP_COOLDOWN;
        goomboss_bounce_mario_high();
        o->oAction = (o->oNumLootCoins <= 0) ? GOOMBOSS_ACT_DEATH : GOOMBOSS_ACT_HIT;
        return;
    }

    // Attack cooldown
    if (o->oGoombossThrowTimer > 0) { o->oGoombossThrowTimer--; }

    // Switch to attack when close and cooldown expired
    if (o->oGoombossThrowTimer == 0 && o->oDistanceToMario < GOOMBOSS_ATTACK_DIST) {
        o->oSubAction = 0;
        o->oAction = GOOMBOSS_ACT_ATTACK;
        return;
    }

    // Walk toward Mario
    o->oForwardVel = (o->oDistanceToMario > GOOMBOSS_ATTACK_DIST) ? GOOMBOSS_FOLLOW_SPEED : 0.0f;
    cur_obj_init_animation_with_accel_and_sound(GOOMBOSS_ANIM_IDLE, 1.0f);
}

// -------------------------------------------------------
// Action: ATTACK — throw-anim swing that hurts Mario
// -------------------------------------------------------

static void goomboss_act_attack(void) {
    o->oForwardVel = 0.0f;
    obj_set_hitbox(o, &sGoombossHitbox);
    cur_obj_rotate_yaw_toward(o->oAngleToMario + GOOMBOSS_MODEL_YAW_OFFSET, GOOMBOSS_FACE_RATE);
    cur_obj_init_animation_with_accel_and_sound(GOOMBOSS_ANIM_THROW, 1.0f);

    // Still allow stomping during attack
    if (o->oGoombossHitCooldown > 0) {
        o->oGoombossHitCooldown--;
    } else if (goomboss_check_stomped()) {
        o->oNumLootCoins--;
        o->oGoombossHitCooldown = GOOMBOSS_STOMP_COOLDOWN;
        goomboss_bounce_mario_high();
        o->oAction = (o->oNumLootCoins <= 0) ? GOOMBOSS_ACT_DEATH : GOOMBOSS_ACT_HIT;
        return;
    }

    // Land the hit on a specific animation frame
    if (o->oSubAction == 0 && cur_obj_check_anim_frame(GOOMBOSS_ATTACK_HIT_FRAME)) {
        if (o->oDistanceToMario < GOOMBOSS_ATTACK_HIT_DIST) {
            goomboss_hurt_mario();
        }
        o->oSubAction = 1;
    }

    // Return to walk once animation finishes
    if (o->oSubAction != 0 && cur_obj_check_if_near_animation_end()) {
        o->oGoombossThrowTimer = GOOMBOSS_ATTACK_COOLDOWN;
        o->oAction = GOOMBOSS_ACT_ACTIVE;
    }
}

// -------------------------------------------------------
// Action: HIT — squish-stun after a stomp
// -------------------------------------------------------

static void goomboss_act_hit(void) {
    o->oForwardVel = 0.0f;

    if (o->oTimer == 0) {
        cur_obj_play_sound_2(SOUND_OBJ2_KING_BOBOMB_DAMAGE);
        cur_obj_shake_screen(SHAKE_POS_SMALL);
        spawn_mist_particles_variable(0, 0, 100.0f);
    }

    cur_obj_rotate_yaw_toward(o->oAngleToMario + GOOMBOSS_MODEL_YAW_OFFSET, GOOMBOSS_FACE_RATE);
    cur_obj_init_animation_with_accel_and_sound(GOOMBOSS_ANIM_IDLE, 1.0f);

    if (o->oTimer >= GOOMBOSS_HIT_STUN_FRAMES) {
        o->oGoombossThrowTimer = GOOMBOSS_ATTACK_COOLDOWN >> 1;
        o->oAction = GOOMBOSS_ACT_ACTIVE;
    }
}

// -------------------------------------------------------
// Action: DEATH — die anim, dialog, spawn star with ID 1
// -------------------------------------------------------

static void goomboss_act_death(void) {
    o->oForwardVel = 0.0f;

    if (o->oTimer == 0) {
        o->oSubAction = 0;
        cur_obj_become_intangible();
        cur_obj_shake_screen(SHAKE_POS_SMALL);
        spawn_mist_particles_variable(0, 0, 200.0f);
        spawn_triangle_break_particles(20, MODEL_DIRT_ANIMATION, 3.0f,
                                       TINY_DIRT_PARTICLE_ANIM_STATE_YELLOW);
    }

    cur_obj_init_animation_with_accel_and_sound(GOOMBOSS_ANIM_DIE, 1.0f);

    if (o->oSubAction == 0) {
        if (cur_obj_check_if_near_animation_end()) { o->oSubAction = 1; }
    } else if (cur_obj_update_dialog(MARIO_DIALOG_LOOK_UP,
               DIALOG_FLAG_TEXT_DEFAULT, GOOMBOSS_DEATH_DIALOG, 0)) {
        spawn_star_with_id(o->oPosX, o->oPosY + 200.0f, o->oPosZ, 1);
        create_sound_spawner(SOUND_OBJ_DEFAULT_DEATH);
        cur_obj_hide();
        o->oAction = GOOMBOSS_ACT_STOP_MUSIC;
    }
}

// -------------------------------------------------------
// Action: STOP_MUSIC
// Let a brief silence play, then fade out the boss music.
// -------------------------------------------------------

static void goomboss_act_stop_music(void) {
    if (o->oTimer == 60) {
        stop_background_music(SEQUENCE_ARGS(4, SEQ_EVENT_BOSS));
    }
}

// -------------------------------------------------------
// Action dispatch table
// -------------------------------------------------------

static ObjActionFunc sGoombossActions[] = {
    goomboss_act_intro,       // 0 GOOMBOSS_ACT_INTRO
    goomboss_act_active,      // 1 GOOMBOSS_ACT_ACTIVE
    goomboss_act_hit,         // 2 GOOMBOSS_ACT_HIT
    goomboss_act_death,       // 3 GOOMBOSS_ACT_DEATH
    goomboss_act_stop_music,  // 4 GOOMBOSS_ACT_STOP_MUSIC
    goomboss_act_attack,      // 5 GOOMBOSS_ACT_ATTACK
};

// -------------------------------------------------------
// Main loop
// -------------------------------------------------------

void bhv_goomboss_loop(void) {
    cur_obj_scale(GOOMBOSS_SCALE);

    cur_obj_update_floor_and_walls();
    cur_obj_move_standard(-78);

    curr_obj_random_blink(&o->oGoombossBlinkTimer);

    cur_obj_call_action_function(sGoombossActions);

    // Squish Y during stomp-stun, recovering linearly back to full scale
    if (o->oAction == GOOMBOSS_ACT_HIT) {
        f32 t = (f32)o->oTimer / (f32)GOOMBOSS_HIT_STUN_FRAMES;
        if (t > 1.0f) t = 1.0f;
        o->header.gfx.scale[1] = GOOMBOSS_SCALE
            * (GOOMBOSS_SQUISH_SCALE_Y + (1.0f - GOOMBOSS_SQUISH_SCALE_Y) * t);
    }

    if (o->oDistanceToMario < 6000.0f) {
        cur_obj_enable_rendering();
    } else {
        cur_obj_disable_rendering();
    }

    o->oInteractStatus = INT_STATUS_NONE;
}
