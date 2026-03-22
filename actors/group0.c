#include <ultra64.h>
#include "sm64.h"
#include "surface_terrains.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"

// Note: This bin does not use bin IDs, unlike the other segmented bins.
#include "mario/export_compat.h"
#include "mario/model.inc.c"

#include "bubble/model.inc.c"

#include "walk_smoke/model.inc.c"

#include "burn_smoke/model.inc.c"

#include "small_water_splash/model.inc.c"

#include "water_wave/model.inc.c"

#include "sparkle/model.inc.c"

#include "water_splash/model.inc.c"

#include "white_particle_small/model.inc.c"

#include "sparkle_animation/model.inc.c"

#ifdef S2DEX_TEXT_ENGINE
#include "src/s2d_engine/s2d_config.h"
#include FONT_C_FILE
#endif
#include "peach_tell/model.inc.c"
#include "peach_tell/collision.inc.c"
#include "bouncingShroom/collision.inc.c"
#include "bouncingShroom/model.inc.c"
#include "bouncePad/collision.inc.c"
#include "bouncePad/model.inc.c"
#include "firePlatform/collision.inc.c"
#include "firePlatform/model.inc.c"
#include "mgMoveBox/collision.inc.c"
#include "mgMoveBox/model.inc.c"
#include "NewPipe/collision.inc.c"
#include "NewPipe/model.inc.c"
#include "FallingRock/collision.inc.c"
#include "FallingRock/model.inc.c"
#include "piranhaalt/collision.inc.c"
#include "piranhaalt/model.inc.c"
#include "pengBoss/model.inc.c"
#include "testyoshi/model.inc.c"
#include "PoliceShy/model.inc.c"
#include "PoliceShy/anims/data.inc.c"
#include "PoliceShy/anims/table.inc.c"
#include "_1StarBarrier/collision.inc.c"
#include "_1StarBarrier/model.inc.c"
#include "goomboss/model.inc.c"
#include "goomboss/anims/data.inc.c"
#include "goomboss/anims/table.inc.c"
#include "boppingFlower/model.inc.c"
#include "boppingFlower/anims/data.inc.c"
#include "boppingFlower/anims/table.inc.c"
#include "c2gate/model.inc.c"
#include "c2gate/collision.inc.c"
