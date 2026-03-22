// Reference the spline array without redefining it
extern const Vec4s wf_area_1_spline_RockPath[];

#include "game/mario.h"
#include "game/object_helpers.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"
#include "game/segment2.h"
#include "game/game_init.h"
#include "game/level_update.h"
#include "game/area.h"
#include "game/save_file.h"
#include "game/camera.h"


extern struct MarioState *gMarioState;
extern u8 gBombButtonCutsceneRequested;

// Forward declarations
void bhv_rgb_light_loop(void);
void bhv_point_light_loop(void);
void bhv_no_sun_loop(void);
void bhv_purple_button_bomb_loop(void);
void bhvC2Gate_loop(void);

#define C2_GATE_ACT_IDLE 0
#define C2_GATE_ACT_LOWERING 1
#define C2_GATE_ACT_LOWERED 2

#define C2_GATE_DEFAULT_DROP_DISTANCE 1800.0f
#define C2_GATE_DROP_SPEED 4.0f

// Global RGB lighting
extern u8 gRGBLightActive;
extern u8 gRGBLightR;
extern u8 gRGBLightG;
extern u8 gRGBLightB;

#define MAX_RGB_LIGHTS 4

typedef struct {
    u8 r, g, b;
    s8 dir[3];
    Vec3f pos;
} RGBLightData;

extern RGBLightData gRGBLights[MAX_RGB_LIGHTS];
extern u8 gRGBLightCount;

extern u32 gLastRGBLightTimer;

// Point light system
#define MAX_POINT_LIGHTS 4

typedef struct {
    u8 r, g, b;
    Vec3f pos;
    f32 radius;
} PointLightData;

extern PointLightData gPointLights[MAX_POINT_LIGHTS];
extern u8 gPointLightCount;
extern u32 gLastPointLightTimer;
extern f32 gPointLightInfluence;
extern u8 gNoSunActive;
extern u32 gLastNoSunTimer;
extern u8 gLensFlareLightActive;
extern u8 gLensFlareLightR;
extern u8 gLensFlareLightG;
extern u8 gLensFlareLightB;
extern Vec3f gLensFlareLightPos;


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

void bhv_bobomb_star_loop(void) {
    switch (o->oAction) {
        case 0:
            o->oHiddenStarTriggerCounter = 0;
            o->oAction = 1;
            break;

        case 1:
            if (o->oHiddenStarTriggerCounter >= 5) {
                spawn_star_with_id(o->oPosX, o->oPosY + 0.0f, o->oPosZ, o->oBehParams2ndByte);
                spawn_mist_particles();
                o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
            }
            break;
    }
}

void bhv_rgb_light_loop(void) {
    // Reset light list once per frame
    if (gGlobalTimer != gLastRGBLightTimer) {
        gRGBLightCount = 0;
        gRGBLightActive = 0;
        gLensFlareLightActive = FALSE;
        gLastRGBLightTimer = gGlobalTimer;
    }

    // Stop if max lights already reached
    if (gRGBLightCount >= MAX_RGB_LIGHTS) {
        return;
    }

    // Extract RGB from behavior parameter
    // Supports both 0x00RRGGBB and 0xRRGGBB00 encodings
    u32 p = (u32)o->oBehParams;
    u8 r;
    u8 g;
    u8 b;

    if ((p & 0xFF) == 0 && ((p >> 8) & 0xFFFFFF) != 0) {
        r = (p >> 24) & 0xFF;
        g = (p >> 16) & 0xFF;
        b = (p >> 8) & 0xFF;
    } else {
        r = (p >> 16) & 0xFF;
        g = (p >> 8) & 0xFF;
        b = (p >> 0) & 0xFF;
    }

    // Store color
    gRGBLights[gRGBLightCount].r = r;
    gRGBLights[gRGBLightCount].g = g;
    gRGBLights[gRGBLightCount].b = b;
    gRGBLightR = r;
    gRGBLightG = g;
    gRGBLightB = b;
    gRGBLights[gRGBLightCount].pos[0] = o->oPosX;
    gRGBLights[gRGBLightCount].pos[1] = o->oPosY;
    gRGBLights[gRGBLightCount].pos[2] = o->oPosZ;
    gLensFlareLightPos[0] = o->oPosX;
    gLensFlareLightPos[1] = o->oPosY;
    gLensFlareLightPos[2] = o->oPosZ;
    gLensFlareLightR = r;
    gLensFlareLightG = g;
    gLensFlareLightB = b;
    gLensFlareLightActive = TRUE;

    // Use light object rotation as directional vector
    s16 yaw = o->oFaceAngleYaw;
    s16 pitch = o->oFaceAnglePitch;
    gRGBLights[gRGBLightCount].dir[0] = (s8)(sins(yaw) * coss(pitch) * 127.0f);
    gRGBLights[gRGBLightCount].dir[1] = (s8)(sins(pitch) * 127.0f);
    gRGBLights[gRGBLightCount].dir[2] = (s8)(coss(yaw) * coss(pitch) * 127.0f);

    // Add light
    gRGBLightCount++;
    gRGBLightActive = 1;
}

void bhv_point_light_loop(void) {
    // Reset point light list once per frame
    if (gGlobalTimer != gLastPointLightTimer) {
        gPointLightCount = 0;
        gLastPointLightTimer = gGlobalTimer;
    }

    // Stop if max lights already reached
    if (gPointLightCount >= MAX_POINT_LIGHTS) {
        return;
    }

    // Extract RGB and radius from behavior parameter
    // Format: 0xRRGGBBRR where low byte is radius
    // Supports RGB in high 24 bits, radius in low 8 bits
    u32 p = (u32)o->oBehParams;
    u8 r = (p >> 24) & 0xFF;
    u8 g = (p >> 16) & 0xFF;
    u8 b = (p >> 8) & 0xFF;
    u8 radiusParam = p & 0xFF;

    // Extract radius (0-255 range, scale to 0-5100)
    f32 radius = (f32)radiusParam * 20.0f;

    // Store point light data
    gPointLights[gPointLightCount].r = r;
    gPointLights[gPointLightCount].g = g;
    gPointLights[gPointLightCount].b = b;
    gPointLights[gPointLightCount].pos[0] = o->oPosX;
    gPointLights[gPointLightCount].pos[1] = o->oPosY;
    gPointLights[gPointLightCount].pos[2] = o->oPosZ;
    gPointLights[gPointLightCount].radius = radius;

    gPointLightCount++;
}

static f32 c2_gate_get_drop_distance(void) {
    u8 dropDistanceParam = GET_BPARAM1(o->oBehParams);

    if (dropDistanceParam != 0) {
        return dropDistanceParam * 50.0f;
    }

    return C2_GATE_DEFAULT_DROP_DISTANCE;
}

void bhvC2Gate_loop(void) {
    f32 targetY = o->oHomeY - c2_gate_get_drop_distance();

    switch (o->oAction) {
        case C2_GATE_ACT_IDLE:
            break;

        case C2_GATE_ACT_LOWERING:
            o->oPosY -= C2_GATE_DROP_SPEED;
            if (o->oPosY <= targetY) {
                o->oPosY = targetY;
                o->oAction = C2_GATE_ACT_LOWERED;
            }
            break;

        case C2_GATE_ACT_LOWERED:
            o->oPosY = targetY;
            break;
    }
}

void bhv_purple_button_bomb_loop(void) {
    switch (o->oAction) {
        case PURPLE_SWITCH_ACT_IDLE:
            cur_obj_set_model(MODEL_PURPLE_SWITCH);
            cur_obj_scale(1.5f);
            if (
                gMarioObject->platform == o
                && !(gMarioStates[0].action & MARIO_NO_PURPLE_SWITCH)
                && lateral_dist_between_objects(o, gMarioObject) < 127.5f
            ) {
                o->oAction = PURPLE_SWITCH_ACT_PRESSED;
            }
            break;

        case PURPLE_SWITCH_ACT_PRESSED:
            cur_obj_scale_over_time(SCALE_AXIS_Y, 3, 1.5f, 0.2f);
            if (o->oTimer == 3) {
                struct Object *gate = cur_obj_nearest_object_with_behavior(bhvC2Gate);

                cur_obj_play_sound_2(SOUND_GENERAL2_PURPLE_SWITCH);
                gBombButtonCutsceneRequested = TRUE;

                if (gate != NULL && gate->oAction == C2_GATE_ACT_IDLE) {
                    gate->oAction = C2_GATE_ACT_LOWERING;
                    play_puzzle_jingle();
                }

                o->oAction = PURPLE_SWITCH_ACT_TICKING;
            }
            break;

        case PURPLE_SWITCH_ACT_TICKING:
            // Already triggered — stay compressed, do nothing
            break;
    }
}

void bhv_no_sun_loop(void) {
    if (gGlobalTimer != gLastNoSunTimer) {
        gNoSunActive = FALSE;
        gLastNoSunTimer = gGlobalTimer;
    }

    gNoSunActive = TRUE;
}

// Rectangular warp zone: BPARAM1=half-width (XZ), BPARAM2=warp node ID, BPARAM3=half-height (Y).
// Teleports Mario instantly (no transition) when he enters the box.
// Each bparam value is multiplied by 50 to get game units (e.g. bparam=4 -> 200 units each side).
// Place the object at the CENTER of the desired trigger volume.
void bhv_instant_warp_zone_loop(void) {
    f32 halfW  = (f32)BPARAM1 * 50.0f;
    f32 halfH  = (f32)BPARAM3 * 50.0f;
    s16 warpId = (s16)BPARAM2;

    f32 dx = gMarioState->pos[0] - o->oPosX;
    f32 dy = gMarioState->pos[1] - o->oPosY;
    f32 dz = gMarioState->pos[2] - o->oPosZ;

    if (absf(dx) < halfW && absf(dy) < halfH && absf(dz) < halfW
            && sDelayedWarpOp == WARP_OP_NONE) {
        gMarioState->invincTimer = -1;
        sDelayedWarpArg          = WARP_FLAGS_NONE;
        sDelayedWarpTimer        = 20;
        sSourceWarpNodeId        = warpId;
        sDelayedWarpOp           = WARP_OP_TELEPORT;
        play_transition(WARP_TRANSITION_FADE_INTO_COLOR, sDelayedWarpTimer, 0xFF, 0xFF, 0xFF);
    }
}

// Warp pipe that also switches the active save file to BPARAM1 (0x01=file1 ... 0x04=file4).
void bhv_save_file_pipe_loop(void) {
    if (o->oInteractStatus & INT_STATUS_INTERACTED) {
        s16 fileNum = (s16)GET_BPARAM1(o->oBehParams);
        if (fileNum >= 1 && fileNum <= 4) {
            load_specific_save(fileNum - 1);
        }
    }
    o->oInteractStatus = INT_STATUS_NONE;
}

void bhv_1_star_barrier_loop(void) {
    if (gMarioState == NULL) {
        return;
    }

    if (gMarioState->numStars >= 1) {
        obj_mark_for_deletion(o);
    }
}