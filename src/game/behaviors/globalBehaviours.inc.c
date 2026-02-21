// Reference the spline array without redefining it
extern const Vec4s wf_area_1_spline_RockPath[];

#include "game/mario.h"
#include "game/object_helpers.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"
#include "game/segment2.h"

#define BOSS_WALK_SPEED 5.0f

//
// ----------------------
// SHROOM BOSS (UNCHANGED)
// ----------------------
//

void bhvShroomBoss_loop(void) {
    struct MarioState *m = gMarioState;
    static s16 health = 3;

    f32 squash = 1.0f;

    f32 dx = m->pos[0] - o->oPosX;
    f32 dz = m->pos[2] - o->oPosZ;
    o->oMoveAngleYaw = atan2s(dz, dx);

    o->oPosX += BOSS_WALK_SPEED * sins(o->oMoveAngleYaw);
    o->oPosZ += BOSS_WALK_SPEED * coss(o->oMoveAngleYaw);

    if (cur_obj_is_mario_on_platform()) {
        if (m->vel[1] < 55.0f) m->vel[1] = 55.0f;
        set_mario_action(m, ACT_TRIPLE_JUMP, 0);

        if (o->oTimer % 10 == 0) health--;

        squash = 0.8f;
    }

    squash += (1.0f - squash) * 0.2f;
    cur_obj_scale(squash);

    if (health <= 0) {
        obj_mark_for_deletion(o);
        spawn_default_star(o->oPosX, o->oPosY + 200.0f, o->oPosZ);
    }
}

//
// ----------------------
// MOVING PLATFORM (UNCHANGED)
// ----------------------
//

void bhvMovingPlatform_loop(void) {
    s32 height = ((o->oBehParams >> 16) & 0xFF) * 10;

    switch (o->oAction) {
        case 0:
            o->oPosY += 8.0f;
            if (o->oPosY >= (o->oHomeY + height)) o->oAction = 1;
            break;

        case 1:
            o->oPosY -= 8.0f;
            if (o->oPosY <= o->oHomeY) o->oAction = 0;
            break;
    }

    load_object_collision_model();
}

//
// ----------------------
// SILVER STAR — EXACT RED COIN LOGIC
// ----------------------
//

u8 gSilverStarTotal = 0;
u8 gSilverStarCount = 0;

static struct ObjectHitbox sSilverStarHitbox = {
    INTERACT_COIN,
    0, 0, 0, 0,
    100, 64,
    0, 0
};

void bhv_silver_star_init(void) {
    if (o->oAction == 0) {
        gSilverStarTotal++;
        o->oAction = 1;
    }

    obj_set_hitbox(o, &sSilverStarHitbox);
}

void bhv_silver_star_loop(void) {
    o->oFaceAngleYaw += 0x400;

    if (o->oInteractStatus & INT_STATUS_INTERACTED) {

        gSilverStarCount++;

        play_sound(SOUND_MENU_COLLECT_RED_COIN, gGlobalSoundSource);

        if (gSilverStarCount >= gSilverStarTotal) {
            s16 starId = (o->oBehParams >> 16) & 0xFF;
            spawn_star_with_id(o->oPosX, o->oPosY + 200.0f, o->oPosZ, starId);
        }

        o->oInteractStatus = INT_STATUS_NONE;
        obj_mark_for_deletion(o);
    }
}


// Penguin Boss 
enum PenguinBossStates {
    PENGUIN_BOSS_STATE_TALK = 0,
    PENGUIN_BOSS_STATE_START_BOSS = 1,
};

#define PENGUIN_BOSS_ACTION_FIGHT 1


void bhv_penguin_boss_message_init(void) {
    // Dialog ID stored in the 2nd behavior param byte
    o->oBehParams2ndByte = GET_BPARAM1(o->oBehParams);

    o->oInteractType = INTERACT_TEXT;
    o->oAction = PENGUIN_BOSS_STATE_TALK;
    o->oIntangibleTimer = 0;
    o->oSubAction = FALSE;

    cur_obj_become_tangible();

}


void bhv_penguin_boss_message_loop(void) {
    switch (o->oAction) {

        case PENGUIN_BOSS_STATE_TALK:
            if (cur_obj_can_mario_activate_textbox_2(300.0f, 150.0f)) {

                if (cur_obj_update_dialog_with_cutscene(
                        MARIO_DIALOG_LOOK_UP,
                        DIALOG_FLAG_TEXT_DEFAULT,
                        CUTSCENE_DIALOG,
                        DIALOG_017)) {

                    o->oAction = PENGUIN_BOSS_STATE_START_BOSS;
                }
            }
            break;

        case PENGUIN_BOSS_STATE_START_BOSS:
            o->oAction = PENGUIN_BOSS_ACTION_FIGHT;
            break;
    }
}
