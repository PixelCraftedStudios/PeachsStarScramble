#include "src/game/envfx_snow.h"

const GeoLayout peach_tell_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, peach_tell_Peach_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, peach_tell_final_revert_mesh_layer_1),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
