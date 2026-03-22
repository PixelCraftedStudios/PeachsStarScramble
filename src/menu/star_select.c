#include "src/game/texscroll.h"
#include <PR/ultratypes.h>

#include "audio/external.h"
#include "behavior_data.h"
#include "engine/math_util.h"
#include "engine/behavior_script.h"
#include "engine/graph_node.h"
#include "eu_translation.h"
#include "game/area.h"
#include "game/game_init.h"
#include "game/ingame_menu.h"
#include "game/level_update.h"
#include "game/memory.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/save_file.h"
#include "game/segment2.h"
#include "game/segment7.h"
#include "game/rumble_init.h"
#include "sm64.h"
#include "star_select.h"
#include "text_strings.h"
#include "game/main.h"

/**
 * @file star_select.c
 * Improved Star Select logic with selection delay and bug fixes.
 */

static struct Object *sStarSelectorModels[8];
static s8 sLoadedActNum;
static u8 sObtainedStars;
static s8 sVisibleStars;
static u8 sInitSelectedActNum;
static s8 sSelectedActIndex = 0;
static s8 sSelectableStarIndex = 0;
static s32 sActSelectorMenuTimer = 0;
static u8 sGradientTopR = 36;
static u8 sGradientTopG = 58;
static u8 sGradientTopB = 110;
static u8 sGradientBottomR = 10;
static u8 sGradientBottomG = 24;
static u8 sGradientBottomB = 54;

#define STAR_SELECT_ROW_Y 160
#define STAR_SELECT_100_COIN_Y 120
#define STAR_SELECT_COURSE_NAME_Y 150
#define STAR_SELECT_COURSE_NUMBER_Y 206
#define STAR_SELECT_ACT_NAME_Y 228

// Internal state to handle the 0.1s exit delay
static s8 sIsActSelected = FALSE;

static void render_star_select_themed_background(void) {
    s16 pulseA = sins(gGlobalTimer * 0x0800);
    s16 pulseB = sins(gGlobalTimer * 0x1200);
    s16 driftA = sins(gGlobalTimer * 0x0400);
    s16 driftB = coss(gGlobalTimer * 0x0600);
    s32 i;
    s32 x;

    for (i = 0; i < 7; i++) {
        f32 t = i / 6.0f;
        u8 r = (u8) (sGradientTopR + (sGradientBottomR - sGradientTopR) * t);
        u8 g = (u8) (sGradientTopG + (sGradientBottomG - sGradientTopG) * t);
        u8 b = (u8) (sGradientTopB + (sGradientBottomB - sGradientTopB) * t);
        f32 y = 218.0f - (i * 30.0f);

        create_dl_translation_matrix(MENU_MTX_PUSH, 160.0f, y, 0.0f);
        create_dl_scale_matrix(MENU_MTX_NOPUSH, 8.2f, 0.78f, 1.0f);
        gDPSetEnvColor(gDisplayListHead++, r, g, b, 44 + (pulseA * 6.0f));
        gSPDisplayList(gDisplayListHead++, dl_draw_text_bg_box);
        gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
    }

    for (i = 0; i < 3; i++) {
        f32 y = 52.0f + (i * 78.0f);
        f32 width = 2.55f - (i * 0.25f);
        f32 height = 0.22f + (i * 0.03f);

        create_dl_translation_matrix(MENU_MTX_PUSH, 160.0f + (driftA * (0.02f + i * 0.01f)), y, 0.0f);
        create_dl_scale_matrix(MENU_MTX_NOPUSH, width, height, 1.0f);
        gDPSetEnvColor(gDisplayListHead++, sGradientTopR + 36, sGradientTopG + 24, sGradientTopB + 12, 56 + (pulseB * 8.0f));
        gSPDisplayList(gDisplayListHead++, dl_draw_text_bg_box);
        gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
    }

    create_dl_translation_matrix(MENU_MTX_PUSH, 160.0f, STAR_SELECT_ROW_Y + 4.0f, 0.0f);
    create_dl_scale_matrix(MENU_MTX_NOPUSH, 2.85f, 0.88f, 1.0f);
    gDPSetEnvColor(gDisplayListHead++, 4, 8, 18, 138);
    gSPDisplayList(gDisplayListHead++, dl_draw_text_bg_box);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    create_dl_translation_matrix(MENU_MTX_PUSH, 160.0f, STAR_SELECT_ROW_Y + 4.0f, 0.0f);
    create_dl_scale_matrix(MENU_MTX_NOPUSH, 2.72f, 0.74f, 1.0f);
    gDPSetEnvColor(gDisplayListHead++, sGradientBottomR + 6, sGradientBottomG + 10, sGradientBottomB + 18, 118);
    gSPDisplayList(gDisplayListHead++, dl_draw_text_bg_box);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    create_dl_translation_matrix(MENU_MTX_PUSH, 160.0f, STAR_SELECT_ROW_Y - 38.0f, 0.0f);
    create_dl_scale_matrix(MENU_MTX_NOPUSH, 2.78f, 0.08f, 1.0f);
    gDPSetEnvColor(gDisplayListHead++, sGradientTopR + 34, sGradientTopG + 28, sGradientTopB + 20, 128 + (pulseB * 8.0f));
    gSPDisplayList(gDisplayListHead++, dl_draw_text_bg_box);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    create_dl_translation_matrix(MENU_MTX_PUSH, 160.0f, STAR_SELECT_ROW_Y + 46.0f, 0.0f);
    create_dl_scale_matrix(MENU_MTX_NOPUSH, 2.62f, 0.05f, 1.0f);
    gDPSetEnvColor(gDisplayListHead++, sGradientTopR + 18, sGradientTopG + 14, sGradientTopB + 10, 92 + (pulseA * 6.0f));
    gSPDisplayList(gDisplayListHead++, dl_draw_text_bg_box);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    for (i = 0; i < 2; i++) {
        x = 78 + (i * 164);
        create_dl_translation_matrix(MENU_MTX_PUSH, x + (i == 0 ? driftA * 0.035f : driftB * 0.03f), STAR_SELECT_ROW_Y + (i == 0 ? -4.0f : 8.0f), 0.0f);
        create_dl_scale_matrix(MENU_MTX_NOPUSH, 0.55f, 1.18f, 1.0f);
        gDPSetEnvColor(gDisplayListHead++, sGradientTopR + 44, sGradientTopG + 34, sGradientTopB + 16, 64 + (pulseA * 10.0f));
        gSPDisplayList(gDisplayListHead++, dl_draw_text_bg_box);
        gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
    }
}

void bhv_act_selector_star_type_loop(void) {
    switch (gCurrentObject->oStarSelectorType) {
        case STAR_SELECTOR_NOT_SELECTED:
            gCurrentObject->oStarSelectorSize -= 0.1f;
            if (gCurrentObject->oStarSelectorSize < 1.0f) {
                gCurrentObject->oStarSelectorSize = 1.0f;
            }
            gCurrentObject->oFaceAngleYaw = 0;
            break;
        case STAR_SELECTOR_SELECTED:
            gCurrentObject->oStarSelectorSize += 0.1f;
            if (gCurrentObject->oStarSelectorSize > 1.3f) {
                gCurrentObject->oStarSelectorSize = 1.3f;
            }
            // If selected, spin faster or keep spinning during delay
            gCurrentObject->oFaceAngleYaw += 0x800;
            break;
        case STAR_SELECTOR_100_COINS:
            gCurrentObject->oFaceAngleYaw += 0x800;
            break;
    }
    cur_obj_scale(gCurrentObject->oStarSelectorSize);
    gCurrentObject->oStarSelectorTimer++;
}

void render_100_coin_star(u8 stars) {
    if (stars & STAR_FLAG_ACT_100_COINS) {
        s16 xPos = 370;
#ifdef WIDE
        if (gConfig.widescreen) xPos = (370 * 4.0f) / 3;
#endif
        sStarSelectorModels[6] = spawn_object_abs_with_rot(o, 0, MODEL_STAR,
                                    bhvActSelectorStarType, xPos, STAR_SELECT_100_COIN_Y, -300, 0, 0, 0);
        sStarSelectorModels[6]->oStarSelectorSize = 0.8f;
        sStarSelectorModels[6]->oStarSelectorType = STAR_SELECTOR_100_COINS;
    }
}


void bhv_act_selector_init(void) {
    s16 i = 0;
    s32 selectorModelIDs[10];
    u8 stars = save_file_get_star_flags(gCurrSaveFileNum - 1, COURSE_NUM_TO_INDEX(gCurrCourseNum));

    sVisibleStars = 0;
    sIsActSelected = FALSE; // Reset selection state

    while (i != sObtainedStars && sVisibleStars < 6) {
        if (stars & (1 << sVisibleStars)) {
            selectorModelIDs[sVisibleStars] = MODEL_STAR;
            i++;
        } else {
            selectorModelIDs[sVisibleStars] = MODEL_TRANSPARENT_STAR;
            if (sInitSelectedActNum == 0) {
                sInitSelectedActNum = sVisibleStars + 1;
                sSelectableStarIndex = sVisibleStars;
            }
        }
        sVisibleStars++;
    }

    if (sVisibleStars == sObtainedStars && sVisibleStars < 6) {
        selectorModelIDs[sVisibleStars] = MODEL_TRANSPARENT_STAR;
        sInitSelectedActNum = sVisibleStars + 1;
        sSelectableStarIndex = sVisibleStars;
        sVisibleStars++;
    }

    if (sObtainedStars == 6) sInitSelectedActNum = 6;
    if (sObtainedStars == 0) sInitSelectedActNum = 1;

    for (i = 0; i < sVisibleStars; i++) {
        s16 xBase = (75 + (sVisibleStars * -75) + (i * 152));
#ifdef WIDE
        if (gConfig.widescreen) xBase = (xBase * 4.0f) / 3;
#endif
        sStarSelectorModels[i] = spawn_object_abs_with_rot(o, 0, selectorModelIDs[i], 
                        bhvActSelectorStarType, xBase, STAR_SELECT_ROW_Y, -300, 0, 0, 0);
        sStarSelectorModels[i]->oStarSelectorSize = 1.0f;
    }

    render_100_coin_star(stars);
}

void bhv_act_selector_loop(void) {
    s8 i;
    u8 starIndexCounter;
    u8 stars = save_file_get_star_flags(gCurrSaveFileNum - 1, COURSE_NUM_TO_INDEX(gCurrCourseNum));

    // Disable scrolling if we've already picked an act
    if (!sIsActSelected) {
        if (sObtainedStars != 6) {
            sSelectedActIndex = 0;
            handle_menu_scrolling(MENU_SCROLL_HORIZONTAL, &sSelectableStarIndex, 0, sObtainedStars);
            starIndexCounter = sSelectableStarIndex;
            for (i = 0; i < sVisibleStars; i++) {
                if ((stars & (1 << i)) || i + 1 == sInitSelectedActNum) {
                    if (starIndexCounter == 0) {
                        sSelectedActIndex = i;
                        break;
                    }
                    starIndexCounter--;
                }
            }
        } else {
            handle_menu_scrolling(MENU_SCROLL_HORIZONTAL, &sSelectableStarIndex, 0, sVisibleStars - 1);
            sSelectedActIndex = sSelectableStarIndex;
        }
    }

    for (i = 0; i < sVisibleStars; i++) {
        if (sSelectedActIndex == i) {
            sStarSelectorModels[i]->oStarSelectorType = STAR_SELECTOR_SELECTED;
        } else {
            sStarSelectorModels[i]->oStarSelectorType = STAR_SELECTOR_NOT_SELECTED;
        }
    }
}

// ... [print_course_number and print_act_selector_strings remain largely the same] ...
#if MULTILANG
void print_course_number(s16 language) {
#else
void print_course_number(void) {
#endif
    u8 courseNum[4];
    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
    int_to_str(gCurrCourseNum, courseNum);
    print_hud_lut_string(HUD_LUT_GLOBAL, (gCurrCourseNum < 10 ? 152 : 143), STAR_SELECT_COURSE_NUMBER_Y, courseNum);
    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
}

void print_act_selector_strings(void) {
#if MULTILANG
    unsigned char myScore[][10] = { {TEXT_MYSCORE}, {TEXT_MY_SCORE_FR}, {TEXT_MY_SCORE_DE} };
    s16 language = eu_get_language();
#else
    unsigned char myScore[] = { TEXT_MYSCORE };
#endif
    unsigned char starNumbers[] = { TEXT_ZERO };
    u8 **levelNameTbl;
    u8 **actNameTbl;
    u8 *currLevelName;
    u8 *selectedActName;
    u8 *courseNameText;
    s8 i;
    s16 titleMenuX;

    create_dl_ortho_matrix();

#if MULTILANG
    switch (language) {
        default:
        case LANGUAGE_ENGLISH: actNameTbl = segmented_to_virtual(act_name_table_eu_en); levelNameTbl = segmented_to_virtual(course_name_table_eu_en); break;
        case LANGUAGE_FRENCH:  actNameTbl = segmented_to_virtual(act_name_table_eu_fr); levelNameTbl = segmented_to_virtual(course_name_table_eu_fr); break;
        case LANGUAGE_GERMAN:  actNameTbl = segmented_to_virtual(act_name_table_eu_de); levelNameTbl = segmented_to_virtual(course_name_table_eu_de); break;
    }
#else
    levelNameTbl = segmented_to_virtual(seg2_course_name_table);
    actNameTbl = segmented_to_virtual(seg2_act_name_table);
#endif
    currLevelName = segmented_to_virtual(levelNameTbl[COURSE_NUM_TO_INDEX(gCurrCourseNum)]);
    courseNameText = currLevelName + 3;

    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
    print_hud_my_score_coins(1, gCurrSaveFileNum - 1, COURSE_NUM_TO_INDEX(gCurrCourseNum), 155, 106);
    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);

    gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
    gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, 255);
    if (save_file_get_course_coin_score(gCurrSaveFileNum - 1, COURSE_NUM_TO_INDEX(gCurrCourseNum)) != 0) {
#if MULTILANG
        print_generic_string(96, 117, myScore[language]);
        gDPSetEnvColor(gDisplayListHead++, 240, 240, 240, 255);
        print_generic_string(95, 118, myScore[language]);
#else
        print_generic_string(103, 117, myScore);
        gDPSetEnvColor(gDisplayListHead++, 240, 240, 240, 255);
        print_generic_string(102, 118, myScore);
#endif
    }
    gSPDisplayList(gDisplayListHead++, dl_ia_text_end);

#if MULTILANG
    print_course_number(language);
#else
    print_course_number();
#endif

    titleMenuX = get_str_x_pos_from_center(160, courseNameText, 8.0f);
    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
    gDPSetEnvColor(gDisplayListHead++, 255,
                   220 + (sins(gGlobalTimer * 0x0D00) * 14.0f),
                   140 + (sins(gGlobalTimer * 0x1100) * 18.0f), 255);
    print_hud_lut_string(HUD_LUT_GLOBAL, titleMenuX, STAR_SELECT_COURSE_NAME_Y, courseNameText);
    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);

    gSPDisplayList(gDisplayListHead++, dl_menu_ia8_text_begin);

    gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, 255);
    if (sVisibleStars != 0) {
        selectedActName = segmented_to_virtual(actNameTbl[COURSE_NUM_TO_INDEX(gCurrCourseNum) * 6 + sSelectedActIndex]);
        print_menu_generic_string(get_str_x_pos_from_center(163, selectedActName, 8.0f) + 1, STAR_SELECT_ACT_NAME_Y, selectedActName);
        gDPSetEnvColor(gDisplayListHead++, 255,
                       232 + (sins(gGlobalTimer * 0x1000) * 10.0f),
                       160 + (sins(gGlobalTimer * 0x0A00) * 18.0f), 255);
        print_menu_generic_string(get_str_x_pos_from_center(163, selectedActName, 8.0f), STAR_SELECT_ACT_NAME_Y + 1, selectedActName);
    }
    for (i = 1; i <= sVisibleStars; i++) {
        starNumbers[0] = i;
        gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, 255);
#if MULTILANG
        print_menu_generic_string(143 - sVisibleStars * 15 + i * 30 + 1, 37, starNumbers);
        gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
        print_menu_generic_string(143 - sVisibleStars * 15 + i * 30, 38, starNumbers);
#else
        print_menu_generic_string(139 - sVisibleStars * 17 + i * 34 + 1, 37, starNumbers);
        gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
        print_menu_generic_string(139 - sVisibleStars * 17 + i * 34, 38, starNumbers);
#endif
    }
    gSPDisplayList(gDisplayListHead++, dl_menu_ia8_text_end);
}

Gfx *geo_act_selector_strings(s16 callContext, UNUSED struct GraphNode *node, UNUSED void *context) {
    if (callContext == GEO_CONTEXT_RENDER) print_act_selector_strings();
    return NULL;
}

s32 lvl_init_act_selector_values_and_stars(UNUSED s32 arg, UNUSED s32 unused) {
    u8 stars = save_file_get_star_flags(gCurrSaveFileNum - 1, COURSE_NUM_TO_INDEX(gCurrCourseNum));
    sLoadedActNum = 0;
    sInitSelectedActNum = 0;
    sVisibleStars = 0;
    sActSelectorMenuTimer = 0;
    sIsActSelected = FALSE;
    sGradientTopR = 58 + (random_u16() & 0x3F);
    sGradientTopG = 58 + (random_u16() & 0x3F);
    sGradientTopB = 58 + (random_u16() & 0x3F);
    sGradientBottomR = 10 + (random_u16() & 0x1F);
    sGradientBottomG = 10 + (random_u16() & 0x1F);
    sGradientBottomB = 10 + (random_u16() & 0x1F);
    sObtainedStars = save_file_get_course_star_count(gCurrSaveFileNum - 1, COURSE_NUM_TO_INDEX(gCurrCourseNum));
    if (stars & STAR_FLAG_ACT_100_COINS) sObtainedStars--;
    return 0;
}

/**
 * UPDATED: Returns act number after a 0.1s (3 frame) delay.
 */
s32 lvl_update_obj_and_load_act_button_actions(UNUSED s32 arg, UNUSED s32 unused) {
    if (sIsActSelected) {
        sActSelectorMenuTimer++;
        // 0.1 seconds = ~3 frames. We wait for the timer to pass the selection point.
        if (sActSelectorMenuTimer > 13) { 
            return sLoadedActNum;
        }
    } else {
        if (sActSelectorMenuTimer > 10) {
            if (gPlayer1Controller->buttonPressed & (A_BUTTON | START_BUTTON | B_BUTTON | Z_TRIG)) {
                play_sound(SOUND_MENU_STAR_SOUND_LETS_A_GO, gGlobalSoundSource);
#if ENABLE_RUMBLE
                queue_rumble_data(60, 70);
                queue_rumble_decay(1);
#endif
                sLoadedActNum = sSelectedActIndex + 1;
                gDialogCourseActNum = sSelectedActIndex + 1;
                sIsActSelected = TRUE;
                sActSelectorMenuTimer = 10; // Reset to 10 so we only wait 3 more frames
            }
        }
        sActSelectorMenuTimer++;
    }

    area_update_objects(); 
    scroll_textures();
    return 0;
}