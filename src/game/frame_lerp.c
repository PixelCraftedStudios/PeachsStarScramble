#include "types.h"
#include "engine/math_util.h"
#include "frame_lerp.h"
#include "main.h"
#include "game_init.h"
#include <PR/os_internal_reg.h>

u32 gFrameLerpRenderFrame;
f32 gFrameLerpDeltaTime;
f32 gDeltaProgress = 0.0f;

#define FRAMELERP_MAX_POS_CACHE 1000
#define FRAMELERP_MAX_ANGULAR_SNAP 2.0f  // radians (~114 degrees)

static inline f32 frameLerp_get_factor(void) {
    // If you later drive this from gFrameLerpDeltaTime, do it here.
    // For now, keep the 0.5f behavior but centralized.
    return 0.5f;
}

f32 *frameLerpPos(Vec3f pos, Vec3f lerpValue) {
    f32 dsq;
    vec3f_get_dist_squared(pos, lerpValue, &dsq);

    // Snap if too far apart to avoid ugly in-betweens.
    if (dsq > 160000.0f) {
        vec3f_copy(lerpValue, pos);
        return lerpValue;
    }

    if (gFrameLerpRenderFrame == FRAMELERP_NORMAL) {
        vec3f_copy(lerpValue, pos);
    } else {
        const f32 t = frameLerp_get_factor();
        Vec3f inBetween;
        // inBetween = lerpValue + (pos - lerpValue) * t
        for (int i = 0; i < 3; i++) {
            inBetween[i] = lerpValue[i] + (pos[i] - lerpValue[i]) * t;
        }
        vec3f_copy(lerpValue, inBetween);
    }

    return lerpValue;
}

f32 *frameLerpRot(Quat rot, Quat lerpValue) {
    // Compute dot and clamp to avoid NaNs in acosf.
    f32 dot = quat_dot(rot, lerpValue);

    // If quaternions are on opposite hemispheres, flip one to ensure shortest path.
    if (dot < 0.0f) {
        dot = -dot;
        Quat flipped;
        flipped[0] = -lerpValue[0];
        flipped[1] = -lerpValue[1];
        flipped[2] = -lerpValue[2];
        flipped[3] = -lerpValue[3];
        quat_copy(lerpValue, flipped);
    }

    if (dot > 1.0f) dot = 1.0f;
    if (dot < -1.0f) dot = -1.0f;

    f32 angle = 2.0f * acosf(dot);

    // If rotation delta is too large, snap to avoid ugly spins / instability.
    if (angle > FRAMELERP_MAX_ANGULAR_SNAP) {
        quat_copy(lerpValue, rot);
        return lerpValue;
    }

    if (gFrameLerpRenderFrame == FRAMELERP_NORMAL) {
        quat_copy(lerpValue, rot);
    } else {
        const f32 t = frameLerp_get_factor();
        Quat inBetween;
        quat_slerp(inBetween, lerpValue, rot, t);
        quat_copy(lerpValue, inBetween);
    }

    return lerpValue;
}

f32 frameLerpFloat(f32 f, f32 lerpValue) {
    if (gFrameLerpRenderFrame == FRAMELERP_NORMAL) {
        return f;
    } else {
        const f32 t = frameLerp_get_factor();
        return lerpValue + (f - lerpValue) * t;
    }
}

static f32 *sCachedPosUpdateRealList[FRAMELERP_MAX_POS_CACHE];
static f32 *sCachedPosUpdateCacheList[FRAMELERP_MAX_POS_CACHE];
static f32 *sCachedPosUpdateCacheVideoList[FRAMELERP_MAX_POS_CACHE];
static int sCachedPosCt = 0;
static int sCachePosTotal = 0;

void frameLerp_cache_pos(f32 *realPosPtr, f32 *cachePosPtr, f32 *cachePosVideoPtr) {
    // Hard guard against overflow; extra registrations are ignored for safety.
    if (sCachedPosCt >= FRAMELERP_MAX_POS_CACHE) {
        return;
    }

    sCachedPosUpdateRealList[sCachedPosCt]       = realPosPtr;
    sCachedPosUpdateCacheList[sCachedPosCt]      = cachePosPtr;
    sCachedPosUpdateCacheVideoList[sCachedPosCt] = cachePosVideoPtr;
    sCachedPosCt++;
}

void frameLerp_update_pos_cache(void) {
    u32 mask = __osDisableInt();

    for (int i = 0; i < sCachedPosCt; i++) {
        if (sCachedPosUpdateRealList[i] && sCachedPosUpdateCacheList[i]) {
            vec3f_copy(sCachedPosUpdateCacheList[i], sCachedPosUpdateRealList[i]);
        }
    }

    sCachePosTotal = sCachedPosCt;
    sCachedPosCt = 0;

    __osRestoreInt(mask);
}

void frameLerp_update_pos_video_cache(void) {
    u32 mask = __osDisableInt();

    for (int i = 0; i < sCachePosTotal; i++) {
        if (sCachedPosUpdateCacheVideoList[i] && sCachedPosUpdateCacheList[i]) {
            vec3f_copy(sCachedPosUpdateCacheVideoList[i], sCachedPosUpdateCacheList[i]);
        }
    }

    __osRestoreInt(mask);
}
