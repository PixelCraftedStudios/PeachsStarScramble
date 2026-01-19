// cutscene_engine.inc.c

#include "levels/bob/header.h"
#include "levels/castle_grounds/header.h"

/* ---------- State ---------- */

static u8 sCutsceneActive = FALSE;
static CustomCutsceneID sCurrentCutscene = CUSTOM_CUTSCENE_NONE;
static u8 sCompletedCutscenes[CUSTOM_CUTSCENE_MAX] = { FALSE };

static s16 sCamSeg = 0, sFocSeg = 0;
static f32 sCamProg = 0.f, sFocProg = 0.f;

static s16 sCurrentEvent = 0;
static s16 sWaitTimer = 0;

static s16 sSplineTimer = 0;

/* ---------- Spline Timing ---------- */

typedef enum {
    MODE_NONE = 0, // spline ends naturally
    MODE_TIME = 1, // spline ends after N frames
} CutsceneSplineMode;

#define TIME(mode, frames) mode, frames


/* ---------- Event Types ---------- */

typedef enum {
    CUTSCENE_EVENT_SPLINE,
    CUTSCENE_EVENT_WAIT,
} CutsceneEventType;

/* ---------- Event Definition ---------- */

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

/* ---------- Cutscene Definition ---------- */

struct CustomCutscene {
    struct CutsceneEvent *events;
    s16 eventCount;
};

/* ---------- Helpers ---------- */
static void reset_cutscene_state(void) {
    sCamSeg = sFocSeg = 0;
    sCamProg = sFocProg = 0.f;
    sCurrentEvent = 0;
    sWaitTimer = 0;
    sSplineTimer = 0;
    sCutsceneActive = TRUE;
}


/* ---------- Cutscene Data ---------- */

/* ---- MH Cutscene ---- */

static struct CutsceneEvent sMHEvents[] = {
    {
        .type = CUTSCENE_EVENT_SPLINE,
        .spline = {
            bob_area_1_spline_mh_cam,
            bob_area_1_spline_mh_foc,
            TIME(MODE_TIME, 60),
        },
    },
    // {
    //     .type = CUTSCENE_EVENT_WAIT,
    //     .wait = { 0 },
    // },
    {
        .type = CUTSCENE_EVENT_SPLINE,
        .spline = {
            bob_area_1_spline_mh_cam2,
            bob_area_1_spline_mh_foc2,
            TIME(MODE_TIME, 60),
        },
    },
    {
        .type = CUTSCENE_EVENT_SPLINE,
        .spline = {
            bob_area_1_spline_mh_cam3,
            bob_area_1_spline_mh_foc3,
            TIME(MODE_TIME, 60),
        },
    },
};

/* ---- Courtyard Cutscene ---- */

static struct CutsceneEvent sCourtyardEvents[] = {
    {
        .type = CUTSCENE_EVENT_SPLINE,
        .spline = {
            castle_grounds_area_1_spline_Cutscene,
            castle_grounds_area_1_spline_Focus,
        },
    },
};

/* ---------- Cutscene Table ---------- */

static struct CustomCutscene sCustomCutscenes[CUSTOM_CUTSCENE_MAX] = {
    [CUSTOM_CUTSCENE_MH] = {
        sMHEvents,
        ARRAY_COUNT(sMHEvents),
    },
    [CUSTOM_CUTSCENE_COURTYARD] = {
        sCourtyardEvents,
        ARRAY_COUNT(sCourtyardEvents),
    },
};

/* ---------- API ---------- */

void StartCutscene(CustomCutsceneID id) {
    if (!gCamera || sCutsceneActive)
        return;

    if (id <= CUSTOM_CUTSCENE_NONE || id >= CUSTOM_CUTSCENE_MAX)
        return;

    if (sCompletedCutscenes[id])
        return;

    sCurrentCutscene = id;
    reset_cutscene_state();

    transition_to_camera_mode(gCamera, CAMERA_MODE_CUTSCENE, 5);
}

/* ---------- Runtime ---------- */

void cutscene_custom_entry(void) {
    if (!gCamera || !sCutsceneActive)
        return;

    struct Camera *c = gCamera;
    struct MarioState *m = &gMarioStates[0];
    struct CustomCutscene *cs = &sCustomCutscenes[sCurrentCutscene];

    c->cutscene = 1;
    set_mario_action(m, ACT_UNUSED_DEATH_EXIT, 0);

    struct CutsceneEvent *ev = &cs->events[sCurrentEvent];

    switch (ev->type) {

        /* ---------- SPLINE EVENT ---------- */
        case CUTSCENE_EVENT_SPLINE: {
    u8 camDone = move_point_along_spline(
        gLakituState.goalPos,
        segmented_to_virtual(ev->spline.camSpline),
        &sCamSeg, &sCamProg
    );

    move_point_along_spline(
        gLakituState.goalFocus,
        segmented_to_virtual(ev->spline.focSpline),
        &sFocSeg, &sFocProg
    );

    // Handle timed splines
    if (ev->spline.mode == MODE_TIME) {
        sSplineTimer++;

        if (sSplineTimer < ev->spline.frames)
            return;
    } else {
        // MODE_NONE: wait for spline completion
        if (!camDone)
            return;
    }

    // Advance event
    sCamSeg = sFocSeg = 0;
    sCamProg = sFocProg = 0.f;
    sSplineTimer = 0;
    sCurrentEvent++;
        } break;


        /* ---------- WAIT EVENT ---------- */
        case CUTSCENE_EVENT_WAIT: {
            if (sWaitTimer < ev->wait.frames) {
                sWaitTimer++;
                return;
            }

            sWaitTimer = 0;
            sCurrentEvent++;
        } break;
    }

    /* ---------- End Cutscene ---------- */

    if (sCurrentEvent < cs->eventCount)
        return;

    sCompletedCutscenes[sCurrentCutscene] = TRUE;
    sCurrentCutscene = CUSTOM_CUTSCENE_NONE;
    sCutsceneActive = FALSE;

    set_mario_action(m, ACT_IDLE, 0);
    set_camera_mode(c, CAMERA_MODE_8_DIRECTIONS, 1);
    c->cutscene = 0;
}
