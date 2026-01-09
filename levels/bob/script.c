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
		OBJECT(MODEL_YELLOW_COIN, -9728, -812, 4514, 0, 0, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, -9178, -812, 4552, 0, 0, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, -8755, -812, 4770, 0, 0, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, -8493, -812, 5136, 0, 0, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, -8377, -812, 5615, 0, 0, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, -8421, -812, 6095, 0, 0, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, -4888, -428, 5186, 0, 0, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, -4290, -231, 5221, 0, 0, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, -3462, -214, 5337, 0, 0, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, -2664, -384, 5427, 0, 0, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, -10164, -788, 3193, 0, 0, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, -1859, -401, 5539, 0, 0, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, 2626, -381, 897, 0, 0, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, 3176, -381, 936, 0, 0, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, 9496, -423, 44, 0, 0, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, 10047, -423, 82, 0, 0, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, 9443, -423, 1366, 0, 0, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, 9993, -423, 1405, 0, 0, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, 10992, -423, 438, 0, -87, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, 10983, -423, 990, 0, -87, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, 8656, -423, 427, 0, -87, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, 8647, -423, 979, 0, -87, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, 5227, -440, -7869, 0, -87, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, 5218, -440, -7317, 0, -87, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, 1908, -440, -9085, 0, -87, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, 1900, -440, -8533, 0, -87, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, -6774, -440, -329, 0, -87, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_YELLOW_COIN, -6783, -440, 223, 0, -87, 0, 0x00000000, bhvYellowCoin),
		OBJECT(MODEL_GOOMBA, -8080, -701, 5050, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_GOOMBA, -9136, -701, 2228, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_GOOMBA, 2278, -441, 3810, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_GOOMBA, -463, -441, 4446, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_GOOMBA, 2118, -441, 1804, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_GOOMBA, 7388, -441, 566, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_GOOMBA, 6332, -441, 5132, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_GOOMBA, 2076, 1650, -3660, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_GOOMBA, 631, 440, -2196, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_NONE, -9433, 80, 5707, 0, 180, 0, (0x0A << 16), bhvSpinAirborneWarp),
		OBJECT(MODEL_STAR, 2580, 2918, -4460, 0, 0, 0, 0x00000000, bhvStar),
		OBJECT(MODEL_STAR, -5176, 4015, -3879, 0, 0, 0, 0x01000000, bhvStar),
		OBJECT(MODEL_STAR, 658, 1881, 7748, 0, 0, 0, 0x02000000, bhvStar),
		OBJECT(MODEL_BOUNCEPAD, 1920, 1471, -3172, 0, -28, 0, 0x00000000, bhvbouncePad),
		OBJECT(MODEL_BOUNCEPAD, 1707, -279, 7760, 0, -28, 0, 0x00000000, bhvbouncePad),
		OBJECT(MODEL_BOUNCEPAD, -3392, -1313, 8129, 0, -28, 0, 0x00000000, bhvbouncePad),
		OBJECT(MODEL_BOUNCINGSHROOM, -2040, 1654, -6282, 0, -75, 0, 0x00000000, bhvbouncingShroom),
		OBJECT(MODEL_BOUNCINGSHROOM, -36, 1258, -7534, 0, 0, 0, 0x00000000, bhvbouncingShroom),
		OBJECT(MODEL_BOUNCINGSHROOM, 2099, 412, 5894, 0, 0, 0, 0x00000000, bhvbouncingShroom),
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
		OBJECT(MODEL_NONE, 0, 80, 0, 0, 0, 0, (0x0A << 16), bhvSpinAirborneWarp),
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