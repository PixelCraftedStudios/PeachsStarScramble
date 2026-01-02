static struct ObjectHitbox sshroombleHitbox = {
    INTERACT_BOUNCE_TOP,
    0,
    1,
    0,
    1,
    72,
    50,
    42,
    40
};

void bhv_shroomble_init(void) {
    o->oGravity = -8.0f / 3.0f;  // simple gravity
    obj_set_hitbox(o, &sshroombleHitbox);
    o->oDamageOrCoinValue = 1;
    o->oDrawingDistance = 4000;
}

static void shroomble_act_walk(void) {
    treat_far_home_as_mario(1000.0f);

    // Basic forward movement
    obj_forward_vel_approach(4.0f / 3.0f, 0.4f);

    // Turn toward Mario if close
    if (o->oDistanceToMario < 500.0f) {
        o->oAngleVelYaw = 0;
        o->oMoveAngleYaw = o->oAngleToMario;
        o->oForwardVel = 20.0f;
    } else {
        // Bounce off walls
        obj_bounce_off_walls_edges_objects(&o->oMoveAngleYaw);
    }

    cur_obj_rotate_yaw_toward(o->oMoveAngleYaw, 0x200);
}

static void shroomble_act_attacked_mario(void) {
    obj_die_if_health_non_positive();
}

void bhv_shroomble_update(void) {
    if (obj_update_standard_actions(1.0f)) {  // no custom scale
        cur_obj_update_floor_and_walls();

        switch (o->oAction) {
            case 0: // walk
                shroomble_act_walk();
                break;
            case 1: // attacked
                shroomble_act_attacked_mario();
                break;
        }

        obj_handle_attacks(&sshroombleHitbox, 1, NULL); // 1 = attacked action
        cur_obj_move_standard(-78);
    }
}
