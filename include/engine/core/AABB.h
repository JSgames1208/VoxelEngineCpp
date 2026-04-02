//
// Created by Jasper Soete on 2-4-2026.
//

#pragma once
#include "engine/core/Vec3i.h"

struct AABB
{
    Vec3i min;
    Vec3i max;

    bool intersects(AABB& other)
    {
        return !(other.min.getX() >= max.getX() || other.max.getX() <= min.getX() ||
                 other.min.getY() >= max.getY() || other.max.getY() <= min.getY() ||
                 other.min.getZ() >= max.getZ() || other.max.getZ() <= min.getZ());
    }

    bool contains(Vec3i& vec)
    {
        return (vec.getX() >= min.getX() && vec.getX() < max.getX() &&
                vec.getY() >= min.getY() && vec.getY() < max.getY() &&
                vec.getZ() >= min.getZ() && vec.getZ() < max.getZ());
    }
};
