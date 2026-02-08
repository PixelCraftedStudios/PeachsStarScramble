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

//
// ----------------------
// CUSTOM TRAJECTORY FOLLOWER (Vec4s version)
// ----------------------
//

// dedicated fields
#define oRockWaypoint o->oF4
#define oRockSpeed    o->oF8

static s32 rock_follow_traj(const Vec4s *trajSeg) {
    const Vec4s *traj = segmented_to_virtual(trajSeg);

    s32 i = (s32)oRockWaypoint;
    if (i < 0) i = oRockWaypoint = 0;

    // end marker
    if (traj[i][0] == 0)
        return TRUE;

    f32 tx = traj[i][1];
    f32 ty = traj[i][2];
    f32 tz = traj[i][3];

    f32 dx = tx - o->oPosX;
    f32 dy = ty - o->oPosY;
    f32 dz = tz - o->oPosZ;

    f32 dist = sqrtf(dx*dx + dy*dy + dz*dz);

    // accelerate like a rolling rock
    oRockSpeed += 1.0f;
    if (oRockSpeed > 45.0f)
        oRockSpeed = 45.0f;

    // if speed would overshoot, snap exactly to the point
    if (oRockSpeed >= dist) {
        o->oPosX = tx;
        o->oPosY = ty;
        o->oPosZ = tz;
        oRockWaypoint++;
        return FALSE;
    }

    // move toward point
    if (dist > 0.0f) {
        f32 inv = 1.0f / dist;
        o->oPosX += dx * inv * oRockSpeed;
        o->oPosY += dy * inv * oRockSpeed;
        o->oPosZ += dz * inv * oRockSpeed;

        o->oMoveAngleYaw = atan2s(dz, dx);
    }

    return FALSE;
}

//
// ----------------------
// FALLING ROCK — CUSTOM TRAJECTORY VERSION
// ----------------------
//

void bhvFallingRock_loop(void) {
    static s32 waitTimer = 0;

    switch (o->oAction) {

        case 0:
            // first frame: snap to first point, reset state
            if (o->oTimer == 0) {
                const Vec4s *traj = segmented_to_virtual(wf_area_1_spline_RockPath);
                oRockWaypoint = 0;
                oRockSpeed = 0.0f;

                o->oPosX = traj[0][1];
                o->oPosY = traj[0][2];
                o->oPosZ = traj[0][3];
            }

            if (rock_follow_traj(wf_area_1_spline_RockPath)) {
                o->oAction = 1;
                waitTimer = 90;
                oRockSpeed = 0.0f;
            }
            break;

        case 1:
            if (--waitTimer <= 0) {
                o->oAction = 0;
                o->oTimer = 0;  // force re-init next time
            }
            break;
    }

    load_object_collision_model();
}
