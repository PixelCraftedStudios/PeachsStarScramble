#include "src/game/envfx_snow.h"

const GeoLayout PoliceShy_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 33, 0, PoliceShy_root_mesh_layer_1),
		GEO_OPEN_NODE(),
			GEO_ANIMATED_PART(LAYER_OPAQUE, -32, 22, 35, PoliceShy_arm_l_mesh_layer_1),
			GEO_ANIMATED_PART(LAYER_OPAQUE, 32, 22, 35, PoliceShy_arm_r_mesh_layer_1),
			GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 78, 0, PoliceShy_hat_mesh_layer_1),
			GEO_ANIMATED_PART(LAYER_OPAQUE, -35, -34, 7, PoliceShy_leg_l_mesh_layer_1),
			GEO_ANIMATED_PART(LAYER_OPAQUE, 35, -34, 7, PoliceShy_leg_r_mesh_layer_1_with_revert),
		GEO_CLOSE_NODE(),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
