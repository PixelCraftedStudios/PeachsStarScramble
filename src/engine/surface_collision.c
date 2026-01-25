#include <PR/ultratypes.h>

#include "sm64.h"
#include "game/debug.h"
#include "game/level_update.h"
#include "game/mario.h"
#include "game/object_list_processor.h"
#include "math_util.h"
#include "surface_collision.h"
#include "surface_load.h"
#include "game/puppyprint.h"

/**************************************************
 *                 RAYCAST HELPERS                *
 **************************************************/

/**
 * Raycast segment P0->P1 against a triangle surface.
 * Returns TRUE if hit within [0,1], writes t and hit point.
 * Uses lateral bounds check consistent with floor/ceil logic.
 */
static s32 raycast_segment_triangle(
    const Vec3f p0,
    const Vec3f p1,
    struct Surface *surf,
    f32 *tOut,
    Vec3f hitOut
) {
    Vec3f dir;
    vec3_diff(dir, p1, p0);

    f32 denom =
        surf->normal.x * dir[0] +
        surf->normal.y * dir[1] +
        surf->normal.z * dir[2];

    if (absf(denom) < 0.0001f) {
        return FALSE; // Parallel or nearly parallel
    }

    f32 t =
        -(surf->normal.x * p0[0] +
          surf->normal.y * p0[1] +
          surf->normal.z * p0[2] +
          surf->originOffset) / denom;

    if (t < 0.0f || t > 1.0f) {
        return FALSE; // Outside segment
    }

    hitOut[0] = p0[0] + dir[0] * t;
    hitOut[1] = p0[1] + dir[1] * t;
    hitOut[2] = p0[2] + dir[2] * t;

    // Lateral bounds check: choose floor vs ceil style based on normal.y
    if (surf->normal.y >= NORMAL_FLOOR_THRESHOLD) {
        // Floor-like
        s32 hx = (s32) hitOut[0];
        s32 hz = (s32) hitOut[2];
        Vec3i vx, vz;
        vx[0] = surf->vertex1[0];
        vz[0] = surf->vertex1[2];
        vx[1] = surf->vertex2[0];
        vz[1] = surf->vertex2[2];

        if (((vz[0] - hz) * (vx[1] - vx[0]) - (vx[0] - hx) * (vz[1] - vz[0])) < 0) return FALSE;

        vx[2] = surf->vertex3[0];
        vz[2] = surf->vertex3[2];

        if (((vz[1] - hz) * (vx[2] - vx[1]) - (vx[1] - hx) * (vz[2] - vz[1])) < 0) return FALSE;
        if (((vz[2] - hz) * (vx[0] - vx[2]) - (vx[2] - hx) * (vz[0] - vz[2])) < 0) return FALSE;
    } else if (surf->normal.y <= NORMAL_CEIL_THRESHOLD) {
        // Ceil-like, no margin
        s32 hx = (s32) hitOut[0];
        s32 hz = (s32) hitOut[2];
        Vec3i vx, vz;
        vx[0] = surf->vertex1[0];
        vz[0] = surf->vertex1[2];
        vx[1] = surf->vertex2[0];
        vz[1] = surf->vertex2[2];

        if (((vz[0] - hz) * (vx[1] - vx[0]) - (vx[0] - hx) * (vz[1] - vz[0])) > 0) return FALSE;

        vx[2] = surf->vertex3[0];
        vz[2] = surf->vertex3[2];

        if (((vz[1] - hz) * (vx[2] - vx[1]) - (vx[1] - hx) * (vz[2] - vz[1])) > 0) return FALSE;
        if (((vz[2] - hz) * (vx[0] - vx[2]) - (vx[2] - hx) * (vz[0] - vz[2])) > 0) return FALSE;
    } else {
        // Wall-like: lateral check using floor-style bounds (XZ)
        s32 hx = (s32) hitOut[0];
        s32 hz = (s32) hitOut[2];
        Vec3i vx, vz;
        vx[0] = surf->vertex1[0];
        vz[0] = surf->vertex1[2];
        vx[1] = surf->vertex2[0];
        vz[1] = surf->vertex2[2];

        if (((vz[0] - hz) * (vx[1] - vx[0]) - (vx[0] - hx) * (vz[1] - vz[0])) < 0) return FALSE;

        vx[2] = surf->vertex3[0];
        vz[2] = surf->vertex3[2];

        if (((vz[1] - hz) * (vx[2] - vx[1]) - (vx[1] - hx) * (vz[2] - vz[1])) < 0) return FALSE;
        if (((vz[2] - hz) * (vx[0] - vx[2]) - (vx[2] - hx) * (vz[0] - vz[2])) < 0) return FALSE;
    }

    *tOut = t;
    return TRUE;
}

/**************************************************
 *                      WALLS                     *
 **************************************************/

/**
 * Compute closest point on segment AB to point P.
 */
static void closest_point_on_segment(Vec3f out, const Vec3f a, const Vec3f b, const Vec3f p) {
    Vec3f ab;
    vec3_diff(ab, b, a);
    f32 abLenSq = vec3_dot(ab, ab);
    if (!FLT_IS_NONZERO(abLenSq)) {
        out[0] = a[0];
        out[1] = a[1];
        out[2] = a[2];
        return;
    }

    Vec3f ap;
    vec3_diff(ap, p, a);
    f32 t = vec3_dot(ap, ab) / abLenSq;
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    out[0] = a[0] + ab[0] * t;
    out[1] = a[1] + ab[1] * t;
    out[2] = a[2] + ab[2] * t;
}

/**
 * Robust two-sided sphere vs triangle collision.
 */
static s32 collide_sphere_with_triangle(
    Vec3f pos,
    f32 radius,
    struct Surface *surf,
    Vec3f push
) {
    f32 dist = surf->normal.x * pos[0]
             + surf->normal.y * pos[1]
             + surf->normal.z * pos[2]
             + surf->originOffset;

    f32 absDist = (dist >= 0.0f) ? dist : -dist;

    if (absDist >= radius) {
        return FALSE;
    }

    Vec3f proj = {
        pos[0] - surf->normal.x * dist,
        pos[1] - surf->normal.y * dist,
        pos[2] - surf->normal.z * dist,
    };

    Vec3f v0 = { surf->vertex1[0], surf->vertex1[1], surf->vertex1[2] };
    Vec3f v1 = { surf->vertex2[0], surf->vertex2[1], surf->vertex2[2] };
    Vec3f v2 = { surf->vertex3[0], surf->vertex3[1], surf->vertex3[2] };

    Vec3f e0, e1, vp;
    vec3_diff(e0, v1, v0);
    vec3_diff(e1, v2, v0);
    vec3_diff(vp, proj, v0);

    f32 d00 = vec3_dot(e0, e0);
    f32 d01 = vec3_dot(e0, e1);
    f32 d11 = vec3_dot(e1, e1);
    f32 d20 = vec3_dot(vp, e0);
    f32 d21 = vec3_dot(vp, e1);

    f32 denom = d00 * d11 - d01 * d01;
    f32 u = 0.0f, v = 0.0f;
    s32 inside = FALSE;

    if (FLT_IS_NONZERO(denom)) {
        f32 invDenom = 1.0f / denom;
        v = (d11 * d20 - d01 * d21) * invDenom;
        f32 w = (d00 * d21 - d01 * d20) * invDenom;
        u = 1.0f - v - w;
        if (u >= 0.0f && v >= 0.0f && w >= 0.0f) {
            inside = TRUE;
        }
    }

    Vec3f closest;
    if (inside) {
        closest[0] = proj[0];
        closest[1] = proj[1];
        closest[2] = proj[2];
    } else {
        Vec3f c0, c1, c2;
        closest_point_on_segment(c0, v0, v1, pos);
        closest_point_on_segment(c1, v1, v2, pos);
        closest_point_on_segment(c2, v2, v0, pos);

        f32 dx, dy, dz;
        f32 d0, d1, d2;

        dx = pos[0] - c0[0];
        dy = pos[1] - c0[1];
        dz = pos[2] - c0[2];
        d0 = dx * dx + dy * dy + dz * dz;

        dx = pos[0] - c1[0];
        dy = pos[1] - c1[1];
        dz = pos[2] - c1[2];
        d1 = dx * dx + dy * dy + dz * dz;

        dx = pos[0] - c2[0];
        dy = pos[1] - c2[1];
        dz = pos[2] - c2[2];
        d2 = dx * dx + dy * dy + dz * dz;

        f32 best = d0;
        closest[0] = c0[0];
        closest[1] = c0[1];
        closest[2] = c0[2];

        if (d1 < best) {
            best = d1;
            closest[0] = c1[0];
            closest[1] = c1[1];
            closest[2] = c1[2];
        }
        if (d2 < best) {
            best = d2;
            closest[0] = c2[0];
            closest[1] = c2[1];
            closest[2] = c2[2];
        }
    }

    f32 dx = pos[0] - closest[0];
    f32 dy = pos[1] - closest[1];
    f32 dz = pos[2] - closest[2];
    f32 distSq = dx * dx + dy * dy + dz * dz;

    if (!FLT_IS_NONZERO(distSq)) {
        f32 penetration = radius - absDist;
        if (penetration <= 0.0f) {
            return FALSE;
        }
        f32 sign = (dist >= 0.0f) ? 1.0f : -1.0f;
        push[0] = surf->normal.x * penetration * sign;
        push[1] = surf->normal.y * penetration * sign;
        push[2] = surf->normal.z * penetration * sign;
        return TRUE;
    }

    f32 distLen = sqrtf(distSq);
    if (distLen >= radius) {
        return FALSE;
    }

    f32 penetration = radius - distLen;
    f32 invLen = 1.0f / distLen;
    dx *= invLen;
    dy *= invLen;
    dz *= invLen;

    push[0] = dx * penetration;
    push[1] = dy * penetration;
    push[2] = dz * penetration;

    return TRUE;
}

/**
 * Iterate through the list of walls and apply robust sphere collision.
 * Raycast helpers are available but movement integration is left to callers,
 * so we only resolve penetration here to avoid speed artifacts.
 */
static s32 find_wall_collisions_from_list(struct SurfaceNode *surfaceNode, struct WallCollisionData *data) {
    const f32 radius = data->radius;
    struct Surface *surf;
    TerrainData type = SURFACE_DEFAULT;
    s32 numCols = 0;

    Vec3f pos = { data->x, data->y + data->offsetY, data->z };

    while (surfaceNode != NULL) {
        surf        = surfaceNode->surface;
        surfaceNode = surfaceNode->next;
        type        = surf->type;

        if (pos[1] < surf->lowerY || pos[1] > surf->upperY) continue;

        // Only treat reasonably vertical surfaces as walls
        if (absf(surf->normal.y) > 0.8f) continue;

        if (gCollisionFlags & COLLISION_FLAG_CAMERA) {
            if (surf->flags & SURFACE_FLAG_NO_CAM_COLLISION) continue;
        } else {
            if (type == SURFACE_CAMERA_BOUNDARY) continue;

            if (type == SURFACE_VANISH_CAP_WALLS && o != NULL) {
                if (o->activeFlags & ACTIVE_FLAG_MOVE_THROUGH_GRATE) continue;
                if (o == gMarioObject && gMarioState->flags & MARIO_VANISH_CAP) continue;
            }
        }

        f32 offset = (surf->normal.x * pos[0])
                   + (surf->normal.y * pos[1])
                   + (surf->normal.z * pos[2])
                   + surf->originOffset;

        if (offset < -radius || offset > radius) continue;

        Vec3f push = { 0.0f, 0.0f, 0.0f };
        if (!collide_sphere_with_triangle(pos, radius, surf, push)) {
            continue;
        }

        // For Mario, ensure horizontal push opposes his motion
        if (o == gMarioObject && gMarioState != NULL) {
            f32 velX = gMarioState->vel[0];
            f32 velZ = gMarioState->vel[2];
            f32 pushX = push[0];
            f32 pushZ = push[2];

            f32 dot = velX * pushX + velZ * pushZ;
            if (dot > 0.0f) {
                push[0] = -push[0];
                push[2] = -push[2];
            }
        }

        // Apply push only in XZ for walls
        pos[0] += push[0];
        pos[2] += push[2];

        if (data->numWalls < MAX_REFERENCED_WALLS) {
            data->walls[data->numWalls++] = surf;
        }
        numCols++;

        if (gCollisionFlags & COLLISION_FLAG_RETURN_FIRST) {
            break;
        }
    }

    data->x = pos[0];
    data->z = pos[2];
    return numCols;
}

/**
 * Formats the position and wall search for find_wall_collisions.
 */
s32 f32_find_wall_collision(f32 *xPtr, f32 *yPtr, f32 *zPtr, f32 offsetY, f32 radius) {
    struct WallCollisionData collision;

    collision.offsetY = offsetY;
    collision.radius = radius;

    collision.x = *xPtr;
    collision.y = *yPtr;
    collision.z = *zPtr;

    collision.numWalls = 0;

    s32 numCollisions = find_wall_collisions(&collision);

    *xPtr = collision.x;
    *yPtr = collision.y;
    *zPtr = collision.z;

    return numCollisions;
}

/**
 * Find wall collisions and receive their push.
 */
s32 find_wall_collisions(struct WallCollisionData *colData) {
    struct SurfaceNode *node;
    s32 numCollisions = 0;
    s32 x = colData->x;
    s32 z = colData->z;
    PUPPYPRINT_ADD_COUNTER(gPuppyCallCounter.collision_wall);
    PUPPYPRINT_GET_SNAPSHOT();

    colData->numWalls = 0;

    if (is_outside_level_bounds(x, z)) {
        profiler_collision_update(first);
        return numCollisions;
    }

    s32 minCellX = GET_CELL_COORD(x - colData->radius);
    s32 minCellZ = GET_CELL_COORD(z - colData->radius);
    s32 maxCellX = GET_CELL_COORD(x + colData->radius);
    s32 maxCellZ = GET_CELL_COORD(z + colData->radius);

    for (s32 cellX = minCellX; cellX <= maxCellX; cellX++) {
        for (s32 cellZ = minCellZ; cellZ <= maxCellZ; cellZ++) {
            if (!(gCollisionFlags & COLLISION_FLAG_EXCLUDE_DYNAMIC)) {
                node = gDynamicSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_WALLS];
                numCollisions += find_wall_collisions_from_list(node, colData);
            }

            node = gStaticSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_WALLS];
            numCollisions += find_wall_collisions_from_list(node, colData);
        }
    }

    gCollisionFlags &= ~(COLLISION_FLAG_RETURN_FIRST | COLLISION_FLAG_EXCLUDE_DYNAMIC | COLLISION_FLAG_INCLUDE_INTANGIBLE);
#ifdef VANILLA_DEBUG
    gNumCalls.wall++;
#endif

    profiler_collision_update(first);
    return numCollisions;
}

/**
 * Collides with walls and returns the most recent wall.
 */
void resolve_and_return_wall_collisions(Vec3f pos, f32 offset, f32 radius, struct WallCollisionData *collisionData) {
    collisionData->x = pos[0];
    collisionData->y = pos[1];
    collisionData->z = pos[2];
    collisionData->radius = radius;
    collisionData->offsetY = offset;

    find_wall_collisions(collisionData);

    pos[0] = collisionData->x;
    pos[1] = collisionData->y;
    pos[2] = collisionData->z;
}

/**************************************************
 *                     CEILINGS                   *
 **************************************************/

void add_ceil_margin(s32 *x, s32 *z, Vec3s target1, Vec3s target2, f32 margin) {
    register f32 diff_x = target1[0] - *x + target2[0] - *x;
    register f32 diff_z = target1[2] - *z + target2[2] - *z;
    register f32 invDenom = margin / sqrtf(sqr(diff_x) + sqr(diff_z));

    *x += diff_x * invDenom;
    *z += diff_z * invDenom;
}

static s32 check_within_ceil_triangle_bounds(s32 x, s32 z, struct Surface *surf, f32 margin) {
    s32 addMargin = surf->type != SURFACE_HANGABLE && !FLT_IS_NONZERO(margin);
    Vec3i vx, vz;
    vx[0] = surf->vertex1[0];
    vz[0] = surf->vertex1[2];
    if (addMargin) add_ceil_margin(&vx[0], &vz[0], surf->vertex2, surf->vertex3, margin);

    vx[1] = surf->vertex2[0];
    vz[1] = surf->vertex2[2];
    if (addMargin) add_ceil_margin(&vx[1], &vz[1], surf->vertex3, surf->vertex1, margin);

    if (((vz[0] - z) * (vx[1] - vx[0]) - (vx[0] - x) * (vz[1] - vz[0])) > 0) return FALSE;

    vx[2] = surf->vertex3[0];
    vz[2] = surf->vertex3[2];
    if (addMargin) add_ceil_margin(&vx[2], &vz[2], surf->vertex1, surf->vertex2, margin);

    if (((vz[1] - z) * (vx[2] - vx[1]) - (vx[1] - x) * (vz[2] - vz[1])) > 0) return FALSE;
    if (((vz[2] - z) * (vx[0] - vx[2]) - (vx[2] - x) * (vz[0] - vz[2])) > 0) return FALSE;

    return TRUE;
}

/**
 * Iterate through ceilings with raycast gate + height logic.
 */
static struct Surface *find_ceil_from_list(struct SurfaceNode *surfaceNode, s32 x, s32 y, s32 z, f32 *pheight) {
    register struct Surface *surf, *ceil = NULL;
    register f32 height;
    SurfaceType type = SURFACE_DEFAULT;
    *pheight = CELL_HEIGHT_LIMIT;

    Vec3f start = { x, y, z };
    Vec3f end   = { x, y + 2000.0f, z }; // upward ray, large enough

    f32 bestT = 1.0f;
    Vec3f bestHit;
    struct Surface *bestSurf = NULL;

    struct SurfaceNode *nodeIter = surfaceNode;
    while (nodeIter != NULL) {
        surf = nodeIter->surface;
        nodeIter = nodeIter->next;
        type = surf->type;

        if (y > surf->upperY) continue;

        if (gCollisionFlags & COLLISION_FLAG_CAMERA) {
            if (surf->flags & SURFACE_FLAG_NO_CAM_COLLISION) {
                continue;
            }
        } else if (type == SURFACE_CAMERA_BOUNDARY) {
            continue;
        }

        f32 t;
        Vec3f hit;
        if (raycast_segment_triangle(start, end, surf, &t, hit)) {
            if (t < bestT) {
                bestT = t;
                bestSurf = surf;
                vec3_copy(bestHit, hit);
            }
        }
    }

    if (bestSurf != NULL) {
        height = bestHit[1];
        if (height <= *pheight && y <= height) {
            *pheight = height;
            ceil = bestSurf;
        }
    }

    if (ceil != NULL && gMarioObject != NULL && gMarioState != NULL) {
        f32 marioY = gMarioState->pos[1];
        f32 clearance = (*pheight) - marioY;

        if (clearance > 0.0f && clearance < 120.0f) {
            s16 yaw = gMarioState->faceAngle[1];
            f32 dirX = sins(yaw);
            f32 dirZ = coss(yaw);

            f32 pushDist = 16.0f;
            f32 pushX = -dirX * pushDist;
            f32 pushZ = -dirZ * pushDist;

            gMarioState->pos[0] += pushX;
            gMarioState->pos[2] += pushZ;
            gMarioObject->oPosX = gMarioState->pos[0];
            gMarioObject->oPosZ = gMarioState->pos[2];
        }
    }

    return ceil;
}

/**
 * Find the lowest ceiling above a given position and return the height.
 */
f32 find_ceil(f32 posX, f32 posY, f32 posZ, struct Surface **pceil) {
    f32 height        = CELL_HEIGHT_LIMIT;
    f32 dynamicHeight = CELL_HEIGHT_LIMIT;
    PUPPYPRINT_ADD_COUNTER(gPuppyCallCounter.collision_ceil);
    PUPPYPRINT_GET_SNAPSHOT();
    s32 x = posX;
    s32 y = posY;
    s32 z = posZ;
    *pceil = NULL;

    if (is_outside_level_bounds(x, z)) {
        profiler_collision_update(first);
        return height;
    }

    s32 cellX = GET_CELL_COORD(x);
    s32 cellZ = GET_CELL_COORD(z);

    struct SurfaceNode *surfaceList;
    struct Surface *ceil = NULL;
    struct Surface *dynamicCeil = NULL;

    s32 includeDynamic = !(gCollisionFlags & COLLISION_FLAG_EXCLUDE_DYNAMIC);

    if (includeDynamic) {
        surfaceList = gDynamicSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_CEILS];
        dynamicCeil = find_ceil_from_list(surfaceList, x, y, z, &dynamicHeight);
        height = dynamicHeight;
    }

    surfaceList = gStaticSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_CEILS];
    ceil = find_ceil_from_list(surfaceList, x, y, z, &height);

    if (includeDynamic && height >= dynamicHeight) {
        ceil   = dynamicCeil;
        height = dynamicHeight;
    }

    gCollisionFlags &= ~(COLLISION_FLAG_RETURN_FIRST | COLLISION_FLAG_EXCLUDE_DYNAMIC | COLLISION_FLAG_INCLUDE_INTANGIBLE);

    *pceil = ceil;
#ifdef VANILLA_DEBUG
    gNumCalls.ceil++;
#endif

    profiler_collision_update(first);
    return height;
}

/**************************************************
 *                     FLOORS                     *
 **************************************************/

static s32 check_within_floor_triangle_bounds(s32 x, s32 z, struct Surface *surf) {
    Vec3i vx, vz;
    vx[0] = surf->vertex1[0];
    vz[0] = surf->vertex1[2];
    vx[1] = surf->vertex2[0];
    vz[1] = surf->vertex2[2];

    if (((vz[0] - z) * (vx[1] - vx[0]) - (vx[0] - x) * (vz[1] - vz[0])) < 0) return FALSE;

    vx[2] = surf->vertex3[0];
    vz[2] = surf->vertex3[2];

    if (((vz[1] - z) * (vx[2] - vx[1]) - (vx[1] - x) * (vz[2] - vz[1])) < 0) return FALSE;
    if (((vz[2] - z) * (vx[0] - vx[2]) - (vx[2] - x) * (vz[0] - vz[2])) < 0) return FALSE;
    return TRUE;
}

/**
 * Iterate through floors with downward raycast gate + height logic.
 */
/**
 * Iterate through the list of floors and find the first floor under a given point.
 * Vanilla-style, robust for all valid floor slopes.
 */
static struct Surface *find_floor_from_list(struct SurfaceNode *surfaceNode, s32 x, s32 y, s32 z, f32 *pheight) {
    register struct Surface *surf, *floor = NULL;
    register SurfaceType type = SURFACE_DEFAULT;
    register f32 height;
    register s32 bufferY = y + FIND_FLOOR_BUFFER;

    // Iterate through the list of floors until there are no more floors.
    while (surfaceNode != NULL) {
        surf = surfaceNode->surface;
        surfaceNode = surfaceNode->next;
        type        = surf->type;

        // Skip intangible floors unless explicitly requested
        if (!(gCollisionFlags & COLLISION_FLAG_INCLUDE_INTANGIBLE) && (type == SURFACE_INTANGIBLE)) {
            continue;
        }

        // Camera collision filtering
        if (gCollisionFlags & COLLISION_FLAG_CAMERA) {
            if (surf->flags & SURFACE_FLAG_NO_CAM_COLLISION) {
                continue;
            }
        } else if (type == SURFACE_CAMERA_BOUNDARY) {
            // Ignore camera-only floors for Mario/objects
            continue;
        }

        // Exclude all floors above the point + buffer
        if (bufferY < surf->lowerY) continue;

        // Check that the point is within the triangle bounds.
        if (!check_within_floor_triangle_bounds(x, z, surf)) continue;

        // Get the height of the floor under the current location.
        height = get_surface_height_at_location(x, z, surf);

        // Exclude floors lower than the previous highest floor.
        if (height <= *pheight) continue;

        // Checks for floor interaction with a FIND_FLOOR_BUFFER unit buffer.
        if (bufferY < height) continue;

        // Use the current floor
        *pheight = height;
        floor = surf;

        // Exit the loop if it's not possible for another floor to be closer
        // to the original point, or if COLLISION_FLAG_RETURN_FIRST.
        if ((height == bufferY) || (gCollisionFlags & COLLISION_FLAG_RETURN_FIRST)) break;
    }

    return floor;
}

// Generic triangle bounds func
ALWAYS_INLINE static s32 check_within_bounds_y_norm(s32 x, s32 z, struct Surface *surf) {
    if (surf->normal.y >= NORMAL_FLOOR_THRESHOLD) return check_within_floor_triangle_bounds(x, z, surf);
    return check_within_ceil_triangle_bounds(x, z, surf, 0);
}

/**
 * Iterate through the list of water floors and find the first water floor under a given point.
 */
struct Surface *find_water_floor_from_list(struct SurfaceNode *surfaceNode, s32 x, s32 y, s32 z, f32 *pheight) {
    register struct Surface *surf;
    struct Surface *floor = NULL;
    struct SurfaceNode *topSurfaceNode = surfaceNode;
    struct SurfaceNode *bottomSurfaceNode = surfaceNode;
    f32 height = FLOOR_LOWER_LIMIT;
    f32 curHeight = FLOOR_LOWER_LIMIT;
    f32 bottomHeight = FLOOR_LOWER_LIMIT;
    f32 curBottomHeight = FLOOR_LOWER_LIMIT;
    f32 buffer = FIND_FLOOR_BUFFER;

    while (bottomSurfaceNode != NULL) {
        surf = bottomSurfaceNode->surface;
        bottomSurfaceNode = bottomSurfaceNode->next;

        if (surf->type != SURFACE_NEW_WATER_BOTTOM || absf(surf->normal.y) < NORMAL_FLOOR_THRESHOLD) continue;

        if (!check_within_bounds_y_norm(x, z, surf)) continue;

        curBottomHeight = get_surface_height_at_location(x, z, surf);

        if (curBottomHeight < y + buffer) {
            continue;
        } else {
            bottomHeight = curBottomHeight;
        }
    }

    while (topSurfaceNode != NULL) {
        surf = topSurfaceNode->surface;
        topSurfaceNode = topSurfaceNode->next;

        if (surf->type == SURFACE_NEW_WATER_BOTTOM || absf(surf->normal.y) < NORMAL_FLOOR_THRESHOLD) continue;

        if (!check_within_bounds_y_norm(x, z, surf)) continue;

        curHeight = get_surface_height_at_location(x, z, surf);

        if (bottomHeight != FLOOR_LOWER_LIMIT && curHeight > bottomHeight) continue;

        if (curHeight > height) {
            height = curHeight;
            *pheight = curHeight;
            floor = surf;
        }
    }

    return floor;
}

/**
 * Find the height of the highest floor below a point.
 */
f32 find_floor_height(f32 x, f32 y, f32 z) {
    struct Surface *floor;
    return find_floor(x, y, z, &floor);
}

/**
 * Find the highest dynamic floor under a given position.
 */
f32 unused_find_dynamic_floor(f32 xPos, f32 yPos, f32 zPos, struct Surface **pfloor) {
    f32 floorHeight = FLOOR_LOWER_LIMIT;

    s32 x = xPos;
    s32 y = yPos;
    s32 z = zPos;

    s32 cellX = GET_CELL_COORD(x);
    s32 cellZ = GET_CELL_COORD(z);

    struct SurfaceNode *surfaceList = gDynamicSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_FLOORS];

    *pfloor = find_floor_from_list(surfaceList, x, y, z, &floorHeight);

    return floorHeight;
}

/**
 * Find the highest floor under a given position and return the height.
 */
f32 find_floor(f32 xPos, f32 yPos, f32 zPos, struct Surface **pfloor) {
    PUPPYPRINT_ADD_COUNTER(gPuppyCallCounter.collision_floor);
    PUPPYPRINT_GET_SNAPSHOT();

    f32 height        = FLOOR_LOWER_LIMIT;
    f32 dynamicHeight = FLOOR_LOWER_LIMIT;

    s32 x = xPos;
    s32 y = yPos;
    s32 z = zPos;

    *pfloor = NULL;

    if (is_outside_level_bounds(x, z)) {
        profiler_collision_update(first);
        return height;
    }

    s32 cellX = GET_CELL_COORD(x);
    s32 cellZ = GET_CELL_COORD(z);

    struct SurfaceNode *surfaceList;
    struct Surface *floor = NULL;
    struct Surface *dynamicFloor = NULL;

    s32 includeDynamic = !(gCollisionFlags & COLLISION_FLAG_EXCLUDE_DYNAMIC);

    if (includeDynamic) {
        surfaceList = gDynamicSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_FLOORS];
        dynamicFloor = find_floor_from_list(surfaceList, x, y, z, &dynamicHeight);
        height = dynamicHeight;
    }

    surfaceList = gStaticSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_FLOORS];
    floor = find_floor_from_list(surfaceList, x, y, z, &height);

    if (includeDynamic && height <= dynamicHeight) {
        floor  = dynamicFloor;
        height = dynamicHeight;
    }

    gCollisionFlags &= ~(COLLISION_FLAG_RETURN_FIRST | COLLISION_FLAG_EXCLUDE_DYNAMIC | COLLISION_FLAG_INCLUDE_INTANGIBLE);
    if (floor == NULL) {
        gNumFindFloorMisses++;
    }

    *pfloor = floor;
#ifdef VANILLA_DEBUG
    gNumCalls.floor++;
#endif

    profiler_collision_update(first);
    return height;
}

f32 find_room_floor(f32 x, f32 y, f32 z, struct Surface **pfloor) {
    gCollisionFlags |= (COLLISION_FLAG_EXCLUDE_DYNAMIC | COLLISION_FLAG_INCLUDE_INTANGIBLE);

    return find_floor(x, y, z, pfloor);
}

/**
 * Get the room index at a given position.
 */
s32 get_room_at_pos(f32 x, f32 y, f32 z) {
    if (gCurrentArea->surfaceRooms != NULL) {
        struct Surface *floor;

        find_room_floor(x, y, z, &floor);

        if (floor != NULL) {
            return floor->room;
        }
    }

    return -1;
}

/**
 * Find the highest water floor under a given position and return the height.
 */
f32 find_water_floor(s32 xPos, s32 yPos, s32 zPos, struct Surface **pfloor) {
    f32 height = FLOOR_LOWER_LIMIT;

    s32 x = xPos;
    s32 y = yPos;
    s32 z = zPos;

    if (is_outside_level_bounds(x, z)) return height;

    s32 cellX = GET_CELL_COORD(x);
    s32 cellZ = GET_CELL_COORD(z);

    struct SurfaceNode *surfaceList = gStaticSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_WATER];
    struct Surface     *floor       = find_water_floor_from_list(surfaceList, x, y, z, &height);

    if (floor == NULL) {
        height = FLOOR_LOWER_LIMIT;
    } else {
        *pfloor = floor;
    }
#ifdef VANILLA_DEBUG
    gNumCalls.floor++;
#endif
    return height;
}

/**************************************************
 *               ENVIRONMENTAL BOXES              *
 **************************************************/

/**
 * Finds the height of water at a given location.
 */
s32 find_water_level_and_floor(s32 x, s32 y, s32 z, struct Surface **pfloor) {
    s32 val;
    s32 loX, hiX, loZ, hiZ;
    TerrainData *p = gEnvironmentRegions;
    struct Surface *floor = NULL;
    PUPPYPRINT_ADD_COUNTER(gPuppyCallCounter.collision_water);
    PUPPYPRINT_GET_SNAPSHOT();
    s32 waterLevel = find_water_floor(x, y, z, &floor);

    if (p != NULL && waterLevel == FLOOR_LOWER_LIMIT) {
        s32 numRegions = *p++;

        for (s32 i = 0; i < numRegions; i++) {
            val = *p++;
            loX = *p++;
            loZ = *p++;
            hiX = *p++;
            hiZ = *p++;

            // Water: val < 50
            if (loX < x && x < hiX && loZ < z && z < hiZ && val < 50) {
                waterLevel = *p;
                break;
            }
            p++;
        }
    } else {
        *pfloor = floor;
    }

    profiler_collision_update(first);
    return waterLevel;
}

/**
 * Finds the height of water at a given location.
 */
s32 find_water_level(s32 x, s32 z) { // TODO: Allow y pos
    s32 val;
    s32 loX, hiX, loZ, hiZ;
    TerrainData *p = gEnvironmentRegions;
    struct Surface *floor = NULL;
    PUPPYPRINT_ADD_COUNTER(gPuppyCallCounter.collision_water);
    PUPPYPRINT_GET_SNAPSHOT();
    s32 waterLevel = find_water_floor(
        x,
        ((gCollisionFlags & COLLISION_FLAG_CAMERA) ? gLakituState.pos[1] : gMarioState->pos[1]),
        z,
        &floor
    );

    if ((p != NULL) && (waterLevel == FLOOR_LOWER_LIMIT)) {
        s32 numRegions = *p++;

        for (s32 i = 0; i < numRegions; i++) {
            val = *p++;
            loX = *p++;
            loZ = *p++;
            hiX = *p++;
            hiZ = *p++;

            if ((loX < x) && (x < hiX) && (loZ < z) && (z < hiZ) && (val < 50)) {
                waterLevel = *p;
                break;
            }
            p++;
        }
    }

    profiler_collision_update(first);
    return waterLevel;
}

/**
 * Find the poison gas level at a given location.
 * Poison gas is stored in environment regions with val >= 50.
 */
s32 find_poison_gas_level(s32 x, s32 z) {
    TerrainData *p = gEnvironmentRegions;
    if (p == NULL) {
        return FLOOR_LOWER_LIMIT;
    }

    s32 numRegions = *p++;

    for (s32 i = 0; i < numRegions; i++) {
        s32 val = *p++;
        s32 loX = *p++;
        s32 loZ = *p++;
        s32 hiX = *p++;
        s32 hiZ = *p++;

        if (loX < x && x < hiX && loZ < z && z < hiZ && val >= 50) {
            s32 gasLevel = *p;
            return gasLevel;
        }
        p++;
    }

    return FLOOR_LOWER_LIMIT;
}
