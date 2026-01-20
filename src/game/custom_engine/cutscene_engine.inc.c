#include "levels/bob/header.h"
#include "levels/castle_grounds/header.h"
#include "game/camera.h"
#include "game/mario.h"
#include "game/segment2.h"

// State
static u8 sCutsceneActive = FALSE;
static CustomCutsceneID sCurrentCutscene = CUSTOM_CUTSCENE_NONE;
static u8 sCompletedCutscenes[CUSTOM_CUTSCENE_MAX] = { FALSE };

static s16 sCamSeg = 0, sFocSeg = 0;
static f32 sCamProg = 0.f, sFocProg = 0.f;

static s16 sCurrentEvent = 0;
static s16 sWaitTimer = 0;
static s16 sSplineTimer = 0;

f32 sLetterboxT = 0.0f;

// Logic - This handles the sliding animation
void update_letterbox(void) {
    const f32 speed = 0.12f;

    if (sCutsceneActive) {
        if (sLetterboxT < 1.0f) {
            sLetterboxT += speed;
            if (sLetterboxT > 1.0f) sLetterboxT = 1.0f;
        }
    } else {
        if (sLetterboxT > 0.0f) {
            sLetterboxT -= speed;
            if (sLetterboxT < 0.0f) sLetterboxT = 0.0f;
        }
    }
}

// Rendering - This just draws the black bars
void render_letterbox(void) {
    s32 barHeight = (s32)(sLetterboxT * 24);
    if (barHeight <= 0) return;

    gDPPipeSync(gDisplayListHead++);
    gDPSetCycleType(gDisplayListHead++, G_CYC_FILL);
    gDPSetRenderMode(gDisplayListHead++, G_RM_NOOP, G_RM_NOOP2);
    gDPSetFillColor(gDisplayListHead++, (GPACK_RGBA5551(0, 0, 0, 1) << 16) | GPACK_RGBA5551(0, 0, 0, 1));

    // Top Bar
    gDPFillRectangle(gDisplayListHead++, 0, 0, SCREEN_WIDTH - 1, barHeight - 1);
    // Bottom Bar
    gDPFillRectangle(gDisplayListHead++, 0, SCREEN_HEIGHT - barHeight, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);

    gDPPipeSync(gDisplayListHead++);
    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
}

// Structures
typedef enum {
    MODE_NONE = 0,
    MODE_TIME = 1,
} CutsceneSplineMode;

#define TIME(mode, frames) mode, frames

typedef enum {
    CUTSCENE_EVENT_SPLINE,
    CUTSCENE_EVENT_WAIT,
} CutsceneEventType;

struct CutsceneEvent {
    CutsceneEventType type;
    union {
        struct {
            struct CutsceneSplinePoint *camSpline;
            struct CutsceneSplinePoint *focSpline;
            CutsceneSplineMode mode;
            s16 frames;
        } spline;
        struct {
            s16 frames;
        } wait;
    };
};

struct CustomCutscene {
    struct CutsceneEvent *events;
    s16 eventCount;
};

static void reset_cutscene_state(void) {
    sCamSeg = sFocSeg = 0;
    sCamProg = sFocProg = 0.f;
    sCurrentEvent = 0;
    sWaitTimer = 0;
    sSplineTimer = 0;
    sCutsceneActive = TRUE;
}

#include "1_cutscenes.inc.c"

// Start
void StartCutscene(CustomCutsceneID id) {
    if (!gCamera || sCutsceneActive) return;
    if (id <= CUSTOM_CUTSCENE_NONE || id >= CUSTOM_CUTSCENE_MAX) return;
    if (sCompletedCutscenes[id]) return;

    sCurrentCutscene = id;
    reset_cutscene_state();
    transition_to_camera_mode(gCamera, CAMERA_MODE_CUTSCENE, 5);
}

// Entry Point
void cutscene_custom_entry(void) {
    if (!gCamera || !sCutsceneActive) return;

    struct Camera *c = gCamera;
    struct MarioState *m = &gMarioStates[0];
    struct CustomCutscene *cs = &sCustomCutscenes[sCurrentCutscene];

    c->cutscene = 1;
    set_mario_action(m, ACT_UNUSED_DEATH_EXIT, 0);

    struct CutsceneEvent *ev = &cs->events[sCurrentEvent];

    switch (ev->type) {
        case CUTSCENE_EVENT_SPLINE: {
            u8 camDone = move_point_along_spline(gLakituState.goalPos, segmented_to_virtual(ev->spline.camSpline), &sCamSeg, &sCamProg);
            move_point_along_spline(gLakituState.goalFocus, segmented_to_virtual(ev->spline.focSpline), &sFocSeg, &sFocProg);

            if (ev->spline.mode == MODE_TIME) {
                sSplineTimer++;
                if (sSplineTimer < ev->spline.frames) return;
            } else if (!camDone) return;

            sCamSeg = sFocSeg = 0;
            sCamProg = sFocProg = 0.f;
            sSplineTimer = 0;
            sCurrentEvent++;
        } break;

        case CUTSCENE_EVENT_WAIT: {
            if (sWaitTimer < ev->wait.frames) {
                sWaitTimer++;
                return;
            }
            sWaitTimer = 0;
            sCurrentEvent++;
        } break;
    }

    if (sCurrentEvent >= cs->eventCount) {
        sCompletedCutscenes[sCurrentCutscene] = TRUE;
        sCurrentCutscene = CUSTOM_CUTSCENE_NONE;
        sCutsceneActive = FALSE; 

        set_mario_action(m, ACT_IDLE, 0);
        set_camera_mode(c, CAMERA_MODE_8_DIRECTIONS, 1);
        c->cutscene = 0;
    }
}