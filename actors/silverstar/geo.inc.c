#include "src/game/envfx_snow.h"

const GeoLayout silverstar_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, silverstar_Cube_mesh_layer_1),
		GEO_BILLBOARD_WITH_PARAMS_AND_DL(LAYER_TRANSPARENT, 0, 0, 0, silverstar_Cube_001_mesh_layer_5),
		GEO_OPEN_NODE(),
			GEO_DISPLAY_LIST(LAYER_TRANSPARENT, silverstar_final_revert_mesh_layer_5),
		GEO_CLOSE_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, silverstar_final_revert_mesh_layer_1),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
