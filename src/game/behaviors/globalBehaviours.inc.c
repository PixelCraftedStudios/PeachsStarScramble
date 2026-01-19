#include "game/mario.h"
#include "game/object_helpers.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"

#define BOSS_WALK_SPEED 5.0f

void bhvShroomBoss_loop(void) {
    struct MarioState *m = gMarioState;
    static s16 health = 3; // simple persistent health

    f32 squash = 1.0f; // local variable for scaling

    // ------------------------
    // Walk towards Mario
    f32 dx = m->pos[0] - o->oPosX;
    f32 dz = m->pos[2] - o->oPosZ;
    o->oMoveAngleYaw = atan2s(dz, dx);

    // move forward manually
    o->oPosX += BOSS_WALK_SPEED * sins(o->oMoveAngleYaw);
    o->oPosZ += BOSS_WALK_SPEED * coss(o->oMoveAngleYaw);

    // ------------------------
    // Mario stomp logic
    if (cur_obj_is_mario_on_platform()) {
        // Bounce Mario up
        if (m->vel[1] < 55.0f) {
            m->vel[1] = 55.0f;
        }

        // Force triple jump animation
        set_mario_action(m, ACT_TRIPLE_JUMP, 0);

        // Reduce health
        if (o->oTimer % 10 == 0) { // prevent multiple hits
            health--;
        }

        // Squash boss visually
        squash = 0.8f;
    }

    // Smoothly return to normal scale
    squash += (1.0f - squash) * 0.2f;
    cur_obj_scale(squash);

    // ------------------------
    // Defeat
    if (health <= 0) {
        obj_mark_for_deletion(o);
        spawn_default_star(o->oPosX, o->oPosY + 200.0f, o->oPosZ);
    }
}
