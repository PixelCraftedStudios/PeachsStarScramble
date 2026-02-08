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
		WARP_NODE(0x02, LEVEL_WF, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_NONE, 2822, -35, -2843, 0, 0, 0, (0x01 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 2626, -44, -2884, 0, 0, 0, (0x01 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 3013, -26, -2804, 0, 0, 0, (0x01 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 3204, -16, -2764, 0, 0, 0, (0x01 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 2823, 165, -2856, 0, 0, 0, (0x01 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 3014, 174, -2816, 0, 0, 0, (0x01 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 3205, 184, -2777, 0, 0, 0, (0x01 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 2628, 156, -2896, 0, 0, 0, (0x01 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 2827, 365, -2861, 0, 0, 0, (0x01 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 3209, 383, -2782, 0, 0, 0, (0x01 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 2631, 355, -2901, 0, 0, 0, (0x01 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 2828, 562, -2866, 0, 0, 0, (0x01 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 3019, 571, -2827, 0, 0, 0, (0x01 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 3210, 580, -2787, 0, 0, 0, (0x01 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 2632, 552, -2907, 0, 0, 0, (0x01 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 1552, 107, -1937, 0, 0, 0, (0x02 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 1583, -63, -1879, 0, 0, 0, (0x02 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 1780, -63, -1962, 0, 0, 0, (0x02 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 1750, 107, -2020, 0, 0, 0, (0x02 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 1361, 128, -1877, 0, 0, 0, (0x02 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 1534, 298, -1970, 0, 0, 0, (0x02 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 1732, 298, -2054, 0, 0, 0, (0x02 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 1344, 319, -1911, 0, 0, 0, (0x02 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 1519, 486, -2004, 0, 0, 0, (0x02 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 1717, 486, -2087, 0, 0, 0, (0x02 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 1328, 507, -1945, 0, 0, 0, (0x02 << 16), bhvWarp),
		MARIO_POS(0x01, 0, 716, -1, 4299),
		OBJECT(MODEL_PEACH_TELL, 941, -204, 3071, 0, -23, 0, (DIALOG_000 << 24), bhvToadMessage),
		OBJECT(MODEL_NONE, 1393, -44, -1817, 0, 0, 0, (0x02 << 16), bhvWarp),
		OBJECT(MODEL_NONE, 714, -123, 4296, 0, 0, 0, (0x0A << 16), bhvSpinAirborneWarp),
		TERRAIN(castle_grounds_area_1_collision),
		MACRO_OBJECTS(castle_grounds_area_1_macro_objs),
		SET_BACKGROUND_MUSIC(0x00, SEQ_OVERWORLD),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),
	FREE_LEVEL_POOL(),
	MARIO_POS(0x01, 0, 716, -1, 4299),
	CALL(0, lvl_init_or_update),
	CALL_LOOP(1, lvl_init_or_update),
	CLEAR_LEVEL(),
	SLEEP_BEFORE_EXIT(1),
	EXIT(),
};