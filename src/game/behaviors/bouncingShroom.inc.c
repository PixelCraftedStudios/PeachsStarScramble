/******************************************************************************
 * bouncingShroom.inc.c
 *
 * Bouncing Mushroom behavior for SM64
 *
 * This module was programmed by Caleb Vernon
 * © PixelCrafted Studios 2026
 * Date: 3rd January 2026
 *
 * Description:
 *   Handles Mario bouncing on mushrooms with squash-and-stretch animation.
 *
 * Notes:
 *   - Uses cur_obj_scale() for uniform scaling.
 *   - Has a currently unfixed bug of scaling all mushrooms in the scene at once.
 ******************************************************************************/
#include "game/mario.h"
#include "game/object_helpers.h"
#include "game/behavior_actions.h"

#define BOUNCE_RADIUS 800.0f
#define BOUNCE_HEIGHT 600.0f

void bhv_bouncingShroom_loop(void) {
    struct MarioState *m = gMarioState;

    // Use oForwardVel as the per-object squash factor
    // (Assuming the mushroom doesn't move, it's safe to repurpose)
    if (o->oForwardVel == 0.0f) {
        o->oForwardVel = 1.0f; // initialize
    }

    f32 squash = o->oForwardVel;

    f32 hDist = lateral_dist_between_objects(o, m->marioObj);
    f32 vDist = m->pos[1] - o->oPosY;

    if (hDist < BOUNCE_RADIUS) {
        if (vDist > (BOUNCE_HEIGHT * 0.8f) && vDist < (BOUNCE_HEIGHT + 50.0f)) {
            if (m->vel[1] < 0.0f) {
                m->vel[1] = (m->vel[1] < 55.0f) ? 55.0f : m->vel[1];
                squash = 0.8f;
            }

            // Force Mario into Triple Jump state/animation
            if (m->action != ACT_TRIPLE_JUMP) {
                set_mario_action(m, ACT_TRIPLE_JUMP, 0);
            } else {
                set_mario_action(m, ACT_TRIPLE_JUMP, 0);
                set_mario_animation(m, ACT_TRIPLE_JUMP);
            }
        }
    }

    // Interpolate squash back to 1.0 (normal size)
    squash += (1.0f - squash) * 0.2f;
    cur_obj_scale(squash);

    // Save back into the object
    o->oForwardVel = squash;
}
