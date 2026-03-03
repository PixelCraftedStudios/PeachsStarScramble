#include "levels/bob/header.h"
#include "levels/castle_grounds/header.h"
#include "levels/wf/header.h"
static struct CutsceneEvent sMHEvents[] = {
    {
        .type = CUTSCENE_EVENT_SPLINE,
        .spline = {
            bob_area_1_spline_mh_cam,
            bob_area_1_spline_mh_foc,
        },
    },
    {
        .type = CUTSCENE_EVENT_WAIT,
        .wait = {
            10,
        },
    },
    {
        .type = CUTSCENE_EVENT_SPLINE,
        .spline = {
            bob_area_1_spline_mh_cam2,
            bob_area_1_spline_mh_foc2,
        },
    },
        {
        .type = CUTSCENE_EVENT_WAIT,
        .wait = {
            10,
        },
    },
    {
        .type = CUTSCENE_EVENT_SPLINE,
        .spline = {
            bob_area_1_spline_mh_cam3,
            bob_area_1_spline_mh_foc3,
        },
    },
        {
        .type = CUTSCENE_EVENT_WAIT,
        .wait = {
            5,
        },
    },
    {
        .type = CUTSCENE_EVENT_SPLINE,
        .spline = {
            bob_area_1_spline_mh_cam4,
            bob_area_1_spline_mh_foc4,
        },
    },
};

static struct CutsceneEvent sCourtyardEvents[] = {
    {
        .type = CUTSCENE_EVENT_SPLINE,
        .spline = { castle_grounds_area_1_spline_Cutscene, castle_grounds_area_1_spline_Focus },
    },
    {
        .type = CUTSCENE_EVENT_WAIT,
        .wait = {
            10,
        },
    },
};
static struct CutsceneEvent sShellshockEvents[] = {
    {
        .type = CUTSCENE_EVENT_SPLINE,
        .spline = { wf_area_1_spline_nePos, wf_area_1_spline_neFoc },
    },
    {
        .type = CUTSCENE_EVENT_SPLINE,
        .spline = { wf_area_1_spline_s_001, wf_area_1_spline_c_001 },
    },
    {
        .type = CUTSCENE_EVENT_SPLINE,
        .spline = { wf_area_1_spline_s_002, wf_area_1_spline_c_002 },
    },
    {
        .type = CUTSCENE_EVENT_SPLINE,
        .spline = { wf_area_1_spline_s_003, wf_area_1_spline_c_003 },
    },
};


static struct CustomCutscene sCustomCutscenes[CUSTOM_CUTSCENE_MAX] = {
    [CUSTOM_CUTSCENE_MH] = { sMHEvents, ARRAY_COUNT(sMHEvents) },
    [CUSTOM_CUTSCENE_COURTYARD] = { sCourtyardEvents, ARRAY_COUNT(sCourtyardEvents) },
    [CUSTOM_CUTSCENE_SHELLSHOCK] = { sShellshockEvents, ARRAY_COUNT(sShellshockEvents) },
};