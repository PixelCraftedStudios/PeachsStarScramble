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
static struct CutsceneEvent sBombButtonEvents[] = {
    {
        .type = CUTSCENE_EVENT_SPLINE,
        .spline = { wf_area_1_spline_oscam, wf_area_1_spline_osfoc },
    },
};
static struct CutsceneEvent sGoombombEvents[] = {
    {
        .type = CUTSCENE_EVENT_SPLINE,
        .spline = { wf_area_1_spline_cam1, wf_area_1_spline_foc1 },
    },
};

static struct CustomCutscene sCustomCutscenes[CUSTOM_CUTSCENE_MAX] = {
    [CUSTOM_CUTSCENE_MH] = { sMHEvents, ARRAY_COUNT(sMHEvents) },
    [CUSTOM_CUTSCENE_COURTYARD] = { sCourtyardEvents, ARRAY_COUNT(sCourtyardEvents) },
    [CUSTOM_CUTSCENE_BOMBBUTTON] = { sBombButtonEvents, ARRAY_COUNT(sBombButtonEvents) },
    [CUSTOM_CUTSCENE_GOOMBOMB] = { sGoombombEvents, ARRAY_COUNT(sGoombombEvents) },

};