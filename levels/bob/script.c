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
		WARP_NODE(0xF0, LEVEL_CASTLE, 0x01, 0x32, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF1, LEVEL_CASTLE, 0x01, 0x64, WARP_NO_CHECKPOINT),
		WARP_NODE(0x0E1, LEVEL_BOB, 0x02, 0x0A, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_YELLOW_COIN, -8651, -71, 5920, 0, 0, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, -8583, -71, 5273, 0, 0, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, -8989, -71, 4809, 0, 0, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, -9588, -71, 4761, 0, 0, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, -9907, -71, 4848, 0, 0, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, -8902, -71, 6423, 0, 0, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, -5731, -62, 4301, 0, 0, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, -5180, -62, 4339, 0, 0, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, -5102, -60, -688, 0, 0, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, -4551, -60, -649, 0, 0, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, -5155, -60, 635, 0, 0, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, -4605, -60, 674, 0, 0, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, -3606, -60, -294, 0, -87, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, -3615, -60, 258, 0, -87, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, -5942, -60, -304, 0, -87, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, -5951, -60, 248, 0, -87, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, -7619, -66, -2499, 0, -87, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, -7628, -66, -1947, 0, -87, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_GOOMBA, -8080, -701, 5050, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_GOOMBA, -9136, -701, 2228, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_GOOMBA, -6908, 169, 1468, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_GOOMBA, -463, -441, 4446, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_GOOMBA, -7069, 169, -539, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_GOOMBA, -1799, 169, -1776, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_GOOMBA, -2855, 169, 2790, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_GOOMBA, 2076, 1650, -3660, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_GOOMBA, 631, 440, -2196, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_NONE, -9433, 80, 5707, 0, 180, 0, (0x0A << 16), bhvSpinAirborneWarp),
		OBJECT(MODEL_STAR, 2580, 2918, -4460, 0, 0, 0, 0x00000000, bhvStar),
		OBJECT(MODEL_STAR, -5176, 4015, -3879, 0, 0, 0, 0x01000000, bhvStar),
		OBJECT(MODEL_STAR, 4691, 451, -5525, 0, 0, 0, 0x02000000, bhvStar),
		OBJECT(MODEL_STAR, -3012, 1680, 3858, 0, 0, 0, 0x02000000, bhvStar),
		OBJECT(MODEL_BOUNCEPAD, 1889, 1268, -3175, 0, -28, 0, 0x00000000, bhvbouncePad),
		OBJECT(MODEL_BOUNCEPAD, -284, 169, -2746, 0, -28, 0, 0x00000000, bhvbouncePad),
		OBJECT(MODEL_BOUNCEPAD, -2650, -136, -1146, 0, -28, 0, 0x00000000, bhvbouncePad),
		OBJECT(MODEL_NONE, -8577, -79, 5265, 0, -28, 0, (0xE1 << 16), bhvWarp),
		OBJECT(MODEL_BOUNCEPAD, -2640, -136, 864, 0, -28, 0, 0x00000000, bhvbouncePad),
		OBJECT(MODEL_BOUNCINGSHROOM, -1970, 1708, -6298, 0, -75, 0, 0x00000000, bhvbouncingShroom),
		OBJECT(MODEL_BOUNCINGSHROOM, -36, 1258, -7534, 0, 0, 0, 0x00000000, bhvbouncingShroom),
		OBJECT(MODEL_BOUNCINGSHROOM, -3110, 353, 2109, 0, 0, 0, 0x00000000, bhvbouncingShroom),
		TERRAIN(bob_area_1_collision),
		MACRO_OBJECTS(bob_area_1_macro_objs),
		SET_BACKGROUND_MUSIC(0x00, SEQ_MUSHROOM_HILL),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),
	AREA(2, bob_area_2),
		WARP_NODE(0x0A, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF0, LEVEL_CASTLE, 0x01, 0x32, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF1, LEVEL_CASTLE, 0x01, 0x64, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_NONE, 30645, 80, 1276, 0, 0, 0, (0x0A << 16), bhvSpinAirborneWarp),
		OBJECT(MODEL_SHROOMBOSS, 30661, 16, -2413, 0, 0, 0, 0x00000000, bhvShroomBoss),
		TERRAIN(bob_area_2_collision),
		MACRO_OBJECTS(bob_area_2_macro_objs),
		SET_BACKGROUND_MUSIC(0x00, SEQ_MUSHROOM_HILL),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),
	FREE_LEVEL_POOL(),
	MARIO_POS(1, 0, 0, 0, 0),
	CALL(0, lvl_init_or_update),
	CALL_LOOP(1, lvl_init_or_update),
	CLEAR_LEVEL(),
	SLEEP_BEFORE_EXIT(1),
	EXIT(),
};