// Reference the spline array without redefining it
extern const Vec4s wf_area_1_spline_RockPath[];

#include "game/mario.h"
#include "game/object_helpers.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"
#include "game/segment2.h"
#include "game/game_init.h"


extern struct MarioState *gMarioState;

// Forward declarations
void bhv_rgb_light_loop(void);
void bhv_point_light_loop(void);

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
        // ACTION 0: Spawn 5 Bob-ombs in a circle
        case 0:
            if (o->oTimer == 0) {
                f32 radius = 300.0f;
                s16 angles[5];

                for (s32 i = 0; i < 5; i++) {
                    s16 angle;
                    s32 valid;
                    s32 attempts = 0;

                    // Pick spaced angles to prevent overlapping
                    do {
                        valid = TRUE;
                        angle = random_u16();
                        for (s32 j = 0; j < i; j++) {
                            u16 diff = (u16)(angle - angles[j]);
                            if (diff > 0x8000) diff = 0x10000 - diff;
                            if (diff < 0x2000) valid = FALSE;
                        }
                        attempts++;
                    } while (!valid && attempts < 20);

                    angles[i] = angle;

                    // Spawn Bob-omb
                    struct Object *bobomb = spawn_object(o, MODEL_BLACK_BOBOMB, bhvBobomb);
                    bobomb->oPosX = o->oPosX + sins(angle) * radius;
                    bobomb->oPosY = o->oPosY;
                    bobomb->oPosZ = o->oPosZ + coss(angle) * radius;

                    bobomb->oHomeX = bobomb->oPosX;
                    bobomb->oHomeY = bobomb->oPosY;
                    bobomb->oHomeZ = bobomb->oPosZ;
                    bobomb->oMoveAngleYaw = angle;

                    // Link the Bob-omb to this spawner
                    bobomb->parentObj = o;
                }
            }
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

    if (!gLensFlareLightActive) {
        gLensFlareLightPos[0] = o->oPosX;
        gLensFlareLightPos[1] = o->oPosY;
        gLensFlareLightPos[2] = o->oPosZ;
        gLensFlareLightR = r;
        gLensFlareLightG = g;
        gLensFlareLightB = b;
        gLensFlareLightActive = TRUE;
    }

    gPointLightCount++;
}