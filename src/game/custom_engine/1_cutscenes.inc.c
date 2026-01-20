#include "levels/bob/header.h"
#include "levels/castle_grounds/header.h"
#include "levels/wf/header.h"

static struct CutsceneEvent sMHEvents[] = {
    {
        .type = CUTSCENE_EVENT_SPLINE,
        .spline = { bob_area_1_spline_mh_cam, bob_area_1_spline_mh_foc, TIME(MODE_TIME, 60) },
    },
    {
        .type = CUTSCENE_EVENT_SPLINE,
        .spline = { bob_area_1_spline_mh_cam2, bob_area_1_spline_mh_foc2, TIME(MODE_TIME, 60) },
    },
    {
        .type = CUTSCENE_EVENT_SPLINE,
        .spline = { bob_area_1_spline_mh_cam3, bob_area_1_spline_mh_foc3, TIME(MODE_TIME, 60) },
    },
};

static struct CutsceneEvent sCourtyardEvents[] = {
    {
        .type = CUTSCENE_EVENT_SPLINE,
        .spline = { castle_grounds_area_1_spline_Cutscene, castle_grounds_area_1_spline_Focus },
    },
};
static struct CutsceneEvent sShellshockEvents[] = {
    {
        .type = CUTSCENE_EVENT_SPLINE,
        .spline = { wf_area_1_spline_cam1, wf_area_1_spline_foc1, TIME(MODE_TIME, 60) },
    },
};


static struct CustomCutscene sCustomCutscenes[CUSTOM_CUTSCENE_MAX] = {
    [CUSTOM_CUTSCENE_MH] = { sMHEvents, ARRAY_COUNT(sMHEvents) },
    [CUSTOM_CUTSCENE_COURTYARD] = { sCourtyardEvents, ARRAY_COUNT(sCourtyardEvents) },
    [CUSTOM_CUTSCENE_SHELLSHOCK] = { sShellshockEvents, ARRAY_COUNT(sShellshockEvents) },
};