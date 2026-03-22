#include <ultra64.h>
#include "sm64.h"
#include "behavior_data.h"
#include "model_ids.h"
#include "seq_ids.h"
#include "dialog_ids.h"
#include "segment_symbols.h"
#include "level_commands.h"

#include "game/level_update.h"

#include "levels/scripts.h"

#include "actors/common1.h"
#include "make_const_nonconst.h"
#include "levels/bob/header.h"

/* Fast64 begin persistent block [scripts] */
/* Fast64 end persistent block [scripts] */

const LevelScript level_bob_entry[] = {
	INIT_LEVEL(),
	LOAD_YAY0(0x07, _bob_segment_7SegmentRomStart, _bob_segment_7SegmentRomEnd), 
	LOAD_YAY0_TEXTURE(0x09, _generic_yay0SegmentRomStart, _generic_yay0SegmentRomEnd), 
	LOAD_YAY0(0x0A, _water_skybox_yay0SegmentRomStart, _water_skybox_yay0SegmentRomEnd), 
	LOAD_YAY0(0x05, _group3_yay0SegmentRomStart, _group3_yay0SegmentRomEnd), 
	LOAD_RAW(0x0C, _group3_geoSegmentRomStart, _group3_geoSegmentRomEnd), 
	LOAD_YAY0(0x06, _group14_yay0SegmentRomStart, _group14_yay0SegmentRomEnd), 
	LOAD_RAW(0x0D, _group14_geoSegmentRomStart, _group14_geoSegmentRomEnd), 
	LOAD_YAY0(0x08, _common0_yay0SegmentRomStart, _common0_yay0SegmentRomEnd), 
	LOAD_RAW(0x0F, _common0_geoSegmentRomStart, _common0_geoSegmentRomEnd), 
	ALLOC_LEVEL_POOL(),
	MARIO(MODEL_MARIO, 0x00000001, bhvMario), 
	JUMP_LINK(script_func_global_1), 
	JUMP_LINK(script_func_global_4), 
	JUMP_LINK(script_func_global_15), 
	LOAD_MODEL_FROM_GEO(MODEL_BOB_BUBBLY_TREE, bubbly_tree_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_BOB_CHAIN_CHOMP_GATE, bob_geo_000440), 
	LOAD_MODEL_FROM_GEO(MODEL_BOB_SEESAW_PLATFORM, bob_geo_000458), 
	LOAD_MODEL_FROM_GEO(MODEL_BOB_BARS_GRILLS, bob_geo_000470), 
	/* Fast64 begin persistent block [level commands] */
	/* Fast64 end persistent block [level commands] */

	AREA(1, bob_area_1),
		WARP_NODE(0x0A, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF0, LEVEL_CASTLE_GROUNDS, 0x02, 0x0C, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF1, LEVEL_CASTLE_GROUNDS, 0x02, 0x0C, WARP_NO_CHECKPOINT),
		WARP_NODE(0x11, LEVEL_BOB, 0x02, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0x12, LEVEL_BOB, 0x03, 0x0A, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_BOPPINGFLOWER, -367, 1757, -3036, 0, -176, 0, 0x00000000, bhvBoppingFlower),
		OBJECT(MODEL_BOPPINGFLOWER, 1249, 1757, -1424, 0, -176, 0, 0x00000000, bhvBoppingFlower),
		OBJECT(MODEL_BOPPINGFLOWER, -2075, 1765, -976, 0, -176, 0, 0x00000000, bhvBoppingFlower),
		OBJECT(MODEL_BOPPINGFLOWER, -3150, -921, 1402, 0, -176, 0, 0x00000000, bhvBoppingFlower),
		OBJECT(MODEL_BOPPINGFLOWER, 742, -895, 4854, 0, -176, 0, 0x00000000, bhvBoppingFlower),
		OBJECT(MODEL_BOPPINGFLOWER, 3225, -923, 1772, 0, -176, 0, 0x00000000, bhvBoppingFlower),
		OBJECT(MODEL_BOPPINGFLOWER, -2709, 2041, 4854, 0, -176, 0, 0x00000000, bhvBoppingFlower),
		OBJECT(MODEL_BOPPINGFLOWER, 3754, 1757, -2877, 0, -176, 0, 0x00000000, bhvBoppingFlower),
		OBJECT(MODEL_EXCLAMATION_BOX, 2775, 2503, -2776, 0, -176, 0, 0x00000000, bhvExclamationBox),
		OBJECT(MODEL_YELLOW_COIN, -1653, 2062, -2355, 0, 0, 0, 0x00000000, bhvOneCoin),
		OBJECT(MODEL_YELLOW_COIN, -208, 2041, -1714, 0, 0, 0, 0x00000000, bhvOneCoin),
		OBJECT(MODEL_YELLOW_COIN, -188, 1757, -924, 0, 0, 0, 0x00000000, bhvOneCoin),
		OBJECT(MODEL_YELLOW_COIN, 631, 1757, -1722, 0, 0, 0, 0x00000000, bhvOneCoin),
		OBJECT(MODEL_YELLOW_COIN, -1025, 1757, -1718, 0, 0, 0, 0x00000000, bhvOneCoin),
		OBJECT(MODEL_YELLOW_COIN, 862, 1757, -3825, 0, 0, 0, 0x00000000, bhvCoinFormation),
		OBJECT(MODEL_YELLOW_COIN, 4313, 2196, 562, 0, 0, 0, 0x00000000, bhvCoinFormation),
		OBJECT(MODEL_GOOMBA, -2433, 2065, -3240, 0, 0, 0, 0x00000000, bhvGoombaTripletSpawner),
		OBJECT(MODEL_GOOMBA, -2628, 2041, 3414, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_GOOMBA, -2393, 2041, 5658, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_GOOMBA, -3176, 2041, 4115, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_GOOMBA, 1028, 2319, 2306, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_BLACK_BOBOMB, -971, 2041, 5362, 0, 0, 0, 0x00000000, bhvBobomb),
		OBJECT(MODEL_GOOMBA, 2709, -891, 2777, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_GOOMBA, -3202, -923, 2841, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_GOOMBA, -4931, -923, 326, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_GOOMBA, -248, -909, 3733, 0, 0, 0, 0x00000000, bhvGoombaTripletSpawner),
		OBJECT(MODEL_GOOMBA, -3589, 1684, 2552, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_GOOMBA, -2746, -970, -1939, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_GOOMBA, -2770, -905, -3544, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_GOOMBA, -613, -1012, -1491, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_GOOMBA, -907, 2041, 3989, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT_WITH_ACTS(MODEL_KING_BOBOMB, -1828, 2041, 4593, 0, -134, 0, 0x00000000, bhvKingBobomb, ACT_1),
		MARIO_POS(0x01, -38, 2389, 1931, -4426),
		OBJECT(MODEL_NEWPIPE, -2444, -863, -4548, 0, 0, 0, (0x11 << 16), bhvWarpPipe),
		OBJECT(MODEL_RED_COIN, 3603, 1846, -702, 0, 0, 0, 0x00000000, bhvRedCoin),
		OBJECT(MODEL_RED_COIN, -2041, 1850, -4879, 0, 0, 0, 0x00000000, bhvRedCoin),
		OBJECT(MODEL_RED_COIN, -2012, 1924, -4145, 0, 0, 0, 0x00000000, bhvRedCoin),
		OBJECT(MODEL_RED_COIN, 628, 2398, 2958, 0, 0, 0, 0x00000000, bhvRedCoin),
		OBJECT(MODEL_RED_COIN, -3429, -923, -4579, 0, 0, 0, 0x00000000, bhvRedCoin),
		OBJECT(MODEL_RED_COIN, -445, -923, -4875, 0, 0, 0, 0x00000000, bhvRedCoin),
		OBJECT(MODEL_RED_COIN, 3842, -923, -241, 0, 0, 0, 0x00000000, bhvRedCoin),
		OBJECT(MODEL_NONE, 414, 2423, 2160, 0, 0, 0, (0x12 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 296, 3090, 2594, 0, 0, 0, (0x02 << 24), bhvHiddenRedCoinStar),
		OBJECT(MODEL_TRANSPARENT_STAR, 283, 2319, 2596, 0, 0, 0, 0x00000000, bhvRedCoinStarMarker),
		OBJECT(MODEL_STAR, 1693, 673, -1306, 0, 0, 0, (0x03 << 24), bhvStar),
		OBJECT(MODEL_STAR, 1790, -791, -730, 0, 0, 0, (0x04 << 24), bhvStar),
		OBJECT(MODEL_NONE, -145, 4959, 10045, -26, 0, 0, 0x00000000, bhvRGBLight),
		OBJECT(MODEL_NONE, 2389, 1931, -4426, 0, -38, 0, (0x0A << 16), bhvAirborneWarp),
		OBJECT(MODEL_MGMOVEBOX, 1785, -903, -378, 0, 0, 0, (300000 << 16), bhvBreakableMHBox),
		OBJECT(MODEL_FIREPLATFORM, -3574, -737, 478, 0, 0, 0, (300000 << 16), bhvFirePlatform),
		OBJECT(MODEL_BOUNCINGSHROOM, -4960, 253, -451, 0, 0, 0, 0x00000000, bhvbouncingShroom),
		TERRAIN(bob_area_1_collision),
		MACRO_OBJECTS(bob_area_1_macro_objs),
		SET_BACKGROUND_MUSIC(0x00, SEQ_MUSHROOM_HILL),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),
	AREA(2, bob_area_2),
		WARP_NODE(0x0A, LEVEL_CASTLE_GROUNDS, 0x02, 0x0C, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF0, LEVEL_CASTLE_GROUNDS, 0x02, 0x0C, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF1, LEVEL_CASTLE, 0x01, 0x64, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_WOODEN_SIGNPOST, -3335, 86, 2808, 0, 0, 0, (0x01 << 16), bhvMessagePanel),
		OBJECT(MODEL_STAR, -3774, 385, -3360, 0, 0, 0, (0x05 << 24), bhvStar),
		OBJECT(MODEL_NONE, -3850, 135, 4381, 59, 0, 0, 0x00000000, bhvRGBLight),
		OBJECT(MODEL_NONE, -3759, 214, 3906, 0, 180, 0, (0x0A << 16), bhvSpinAirborneWarp),
		TERRAIN(bob_area_2_collision),
		MACRO_OBJECTS(bob_area_2_macro_objs),
		SET_BACKGROUND_MUSIC(0x00, SEQ_LEVEL_UNDERGROUND),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),
	AREA(3, bob_area_3),
		WARP_NODE(0x0A, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF0, LEVEL_CASTLE_GROUNDS, 0x02, 0x0C, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF1, LEVEL_CASTLE_GROUNDS, 0x02, 0x0C, WARP_NO_CHECKPOINT),
		WARP_NODE(0x11, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_NONE, 20, 301, 1784, 0, 0, 0, (0x11 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 141, 2196, 14, -63, -1, -179, 0x00000000, bhvRGBLight),
		OBJECT(MODEL_NONE, 6, 159, 1046, 0, 180, 0, (0x0A << 16), bhvSpinAirborneWarp),
		OBJECT(MODEL_STAR, 23, 1296, -2094, 0, 0, 0, (0x01 << 24), bhvStar),
		TERRAIN(bob_area_3_collision),
		MACRO_OBJECTS(bob_area_3_macro_objs),
		SET_BACKGROUND_MUSIC(0x00, SEQ_MARIOHOUSE),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),
	FREE_LEVEL_POOL(),
	MARIO_POS(0x01, -38, 2389, 1931, -4426),
	CALL(0, lvl_init_or_update),
	CALL_LOOP(1, lvl_init_or_update),
	CLEAR_LEVEL(),
	SLEEP_BEFORE_EXIT(1),
	EXIT(),
};