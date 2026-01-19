#include "game/mario.h"
#include "game/object_helpers.h"
#include "game/behavior_actions.h"

#define BOUNCE_RADIUS 800.0f
#define BOUNCE_HEIGHT 600.0f

void bhv_bouncingShroom_loop(void) {
    struct MarioState *m = gMarioState;


    // init squash storage
    if (o->oForwardVel == 0.0f) {
        o->oForwardVel = 1.0f;
    }

    f32 squash = o->oForwardVel;

    if (cur_obj_is_mario_on_platform()) {
        // Bounce Mario up
        if (m->vel[1] < 55.0f) {
            m->vel[1] = 55.0f;
        }

        // Force triple jump
        set_mario_action(m, ACT_TRIPLE_JUMP, 0);
        set_mario_animation(m, ACT_TRIPLE_JUMP);

        // Squash mushroom
        squash = 0.8f;
    }

    // Smoothly return to normal scale
    squash += (1.0f - squash) * 0.2f;
    cur_obj_scale(squash);

    o->oForwardVel = squash;
}
