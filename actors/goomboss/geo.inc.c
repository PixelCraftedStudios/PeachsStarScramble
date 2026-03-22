#include "src/game/envfx_snow.h"

const GeoLayout goomboss_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 45, 0, NULL),
		GEO_OPEN_NODE(),
			GEO_BILLBOARD_WITH_PARAMS_AND_DL(LAYER_ALPHA, 0, -36, 0, goomboss_Bone_mesh_layer_4),
			GEO_ANIMATED_PART(LAYER_ALPHA, 0, 22, 0, goomboss_crown_mesh_layer_4),
			GEO_OPEN_NODE(),
				GEO_DISPLAY_LIST(LAYER_OPAQUE, goomboss_crown_mesh_layer_1),
			GEO_CLOSE_NODE(),
			GEO_ANIMATED_PART(LAYER_OPAQUE, 0, -17, 0, goomboss_head_mesh_layer_1),
			GEO_OPEN_NODE(),
				GEO_DISPLAY_LIST(LAYER_ALPHA, goomboss_head_mesh_layer_4_with_revert),
			GEO_CLOSE_NODE(),
			GEO_ANIMATED_PART(LAYER_OPAQUE, 0, -26, -6, goomboss_leg_l_mesh_layer_1),
			GEO_ANIMATED_PART(LAYER_OPAQUE, 0, -26, 6, goomboss_leg_r_mesh_layer_1_with_revert),
		GEO_CLOSE_NODE(),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
