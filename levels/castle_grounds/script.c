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
#include "levels/castle_grounds/header.h"

/* Fast64 begin persistent block [scripts] */
/* Fast64 end persistent block [scripts] */

const LevelScript level_castle_grounds_entry[] = {
	INIT_LEVEL(),
	LOAD_YAY0(0x07, _castle_grounds_segment_7SegmentRomStart, _castle_grounds_segment_7SegmentRomEnd), 
	LOAD_YAY0(0x0A, _cloud_floor_skybox_yay0SegmentRomStart, _cloud_floor_skybox_yay0SegmentRomEnd), 
	LOAD_YAY0_TEXTURE(0x09, _outside_yay0SegmentRomStart, _outside_yay0SegmentRomEnd), 
	LOAD_YAY0(0x05, _group10_yay0SegmentRomStart, _group10_yay0SegmentRomEnd), 
	LOAD_RAW(0x0C, _group10_geoSegmentRomStart, _group10_geoSegmentRomEnd), 
	LOAD_YAY0(0x06, _group15_yay0SegmentRomStart, _group15_yay0SegmentRomEnd), 
	LOAD_RAW(0x0D, _group15_geoSegmentRomStart, _group15_geoSegmentRomEnd), 
	LOAD_YAY0(0x08, _common0_yay0SegmentRomStart, _common0_yay0SegmentRomEnd), 
	LOAD_RAW(0x0F, _common0_geoSegmentRomStart, _common0_geoSegmentRomEnd), 
	ALLOC_LEVEL_POOL(),
	MARIO(MODEL_MARIO, 0x00000001, bhvMario), 
	JUMP_LINK(script_func_global_1), 
	JUMP_LINK(script_func_global_11), 
	JUMP_LINK(script_func_global_16), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_03, castle_grounds_geo_0006F4), 
	LOAD_MODEL_FROM_GEO(MODEL_CASTLE_GROUNDS_BUBBLY_TREE, bubbly_tree_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_CASTLE_GROUNDS_WARP_PIPE, warp_pipe_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_CASTLE_GROUNDS_CASTLE_DOOR, castle_door_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_CASTLE_GROUNDS_METAL_DOOR, metal_door_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_CASTLE_GROUNDS_VCUTM_GRILL, castle_grounds_geo_00070C), 
	LOAD_MODEL_FROM_GEO(MODEL_CASTLE_GROUNDS_FLAG, castle_grounds_geo_000660), 
	LOAD_MODEL_FROM_GEO(MODEL_CASTLE_GROUNDS_CANNON_GRILL, castle_grounds_geo_000724), 
	/* Fast64 begin persistent block [level commands] */
	/* Fast64 end persistent block [level commands] */

	AREA(1, castle_grounds_area_1),
		WARP_NODE(0x0A, LEVEL_CASTLE_GROUNDS, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF0, LEVEL_CASTLE_GROUNDS, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF1, LEVEL_CASTLE_GROUNDS, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0x01, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0x03, LEVEL_CASTLE_GROUNDS, 0x02, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0x0B, LEVEL_CASTLE_GROUNDS, 0x01, 0x0B, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_NONE, 4331, -209, 282, 0, -1, 0, (0x03 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 4335, -209, 84, 0, -1, 0, (0x03 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 4341, -209, -109, 0, -1, 0, (0x03 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 4348, -209, -303, 0, -1, 0, (0x03 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 4356, -209, -500, 0, -1, 0, (0x03 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 4364, -209, -698, 0, -1, 0, (0x03 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 4374, -209, -894, 0, -1, 0, (0x03 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 4325, -95, 274, 0, -1, 0, (0x03 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 4329, -95, 76, 0, -1, 0, (0x03 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 4336, -95, -116, 0, -1, 0, (0x03 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 4342, -95, -311, 0, -1, 0, (0x03 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 4350, -95, -507, 0, -1, 0, (0x03 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 4358, -95, -705, 0, -1, 0, (0x03 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 4368, -95, -901, 0, -1, 0, (0x03 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 4319, 6, 272, 0, -1, 0, (0x03 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 4324, 6, 74, 0, -1, 0, (0x03 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 4330, 6, -119, 0, -1, 0, (0x03 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 4337, 6, -313, 0, -1, 0, (0x03 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 3344, -116, -291, 0, -96, 0, (0x0B << 16), bhvAirborneWarp),
		MARIO_POS(0x01, -178, -699, -169, 2507),
		OBJECT(MODEL_PEACH_TELL, -761, -221, 1836, 0, -23, 0, (DIALOG_000 << 24), bhvToadMessage),
		OBJECT(MODEL_NONE, -697, -169, 2510, 0, -178, 0, (0x0A << 16), bhvSpinAirborneWarp),
		TERRAIN(castle_grounds_area_1_collision),
		MACRO_OBJECTS(castle_grounds_area_1_macro_objs),
		SET_BACKGROUND_MUSIC(0x00, SEQ_OVERWORLD),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),
	AREA(2, castle_grounds_area_2),
		WARP_NODE(0x0A, LEVEL_CASTLE_GROUNDS, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF0, LEVEL_CASTLE_GROUNDS, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF1, LEVEL_CASTLE_GROUNDS, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0x01, LEVEL_CASTLE_GROUNDS, 0x01, 0x0B, WARP_NO_CHECKPOINT),
		WARP_NODE(0x0B, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0x0C, LEVEL_CASTLE_GROUNDS, 0x02, 0x0A, WARP_NO_CHECKPOINT),
		MARIO_POS(0x02, 1, -510, 416, -2617),
		OBJECT(MODEL_NONE, -2921, 887, 258, 0, -92, 0, (0x0C << 16), bhvAirborneStarCollectWarp),
		OBJECT(MODEL_NONE, -502, 89, -2617, 0, 1, 0, (0x0A << 16), bhvSpinAirborneWarp),
		OBJECT(MODEL_NONE, -782, 97, -3230, 0, 0, 0, (0x01 << 16), bhvWarp),
		OBJECT(MODEL_NONE, -684, 97, -3233, 0, 0, 0, (0x01 << 16), bhvWarp),
		OBJECT(MODEL_NONE, -580, 97, -3232, 0, 0, 0, (0x01 << 16), bhvWarp),
		OBJECT(MODEL_NONE, -480, 97, -3235, 0, 0, 0, (0x01 << 16), bhvWarp),
		OBJECT(MODEL_NONE, -385, 97, -3238, 0, 0, 0, (0x01 << 16), bhvWarp),
		OBJECT(MODEL_NONE, -279, 97, -3239, 0, 0, 0, (0x01 << 16), bhvWarp),
		OBJECT(MODEL_NONE, -186, 97, -3239, 0, 0, 0, (0x01 << 16), bhvWarp),
		OBJECT(MODEL_NONE, -80, 97, -3239, 0, 0, 0, (0x01 << 16), bhvWarp),
		OBJECT(MODEL_NONE, -884, 97, -3228, 0, 0, 0, (0x01 << 16), bhvWarp),
		OBJECT(MODEL_NONE, -2185, -96, 562, 0, 0, 0, (0x0B << 16), bhvWarp),
		OBJECT(MODEL_NONE, -2000, -92, 569, 0, 0, 0, (0x0B << 16), bhvWarp),
		OBJECT(MODEL_NONE, -1793, -87, 577, 0, 0, 0, (0x0B << 16), bhvWarp),
		OBJECT(MODEL_NONE, -1599, -91, 581, 0, 0, 0, (0x0B << 16), bhvWarp),
		OBJECT(MODEL_NONE, -1620, 86, 616, 0, 0, 0, (0x0B << 16), bhvWarp),
		OBJECT(MODEL_NONE, -1818, 91, 600, 0, 0, 0, (0x0B << 16), bhvWarp),
		OBJECT(MODEL_NONE, -2017, 93, 582, 0, 0, 0, (0x0B << 16), bhvWarp),
		OBJECT(MODEL_NONE, -2217, 100, 565, 0, 0, 0, (0x0B << 16), bhvWarp),
		TERRAIN(castle_grounds_area_2_collision),
		MACRO_OBJECTS(castle_grounds_area_2_macro_objs),
		SET_BACKGROUND_MUSIC(0x00, SEQ_MARIOHOUSE),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),
	FREE_LEVEL_POOL(),
	MARIO_POS(0x02, 1, -510, 416, -2617),
	CALL(0, lvl_init_or_update),
	CALL_LOOP(1, lvl_init_or_update),
	CLEAR_LEVEL(),
	SLEEP_BEFORE_EXIT(1),
	EXIT(),
};