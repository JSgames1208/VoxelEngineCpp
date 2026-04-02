//
// Created by Jasper Soete on 2-4-2026.
//

#pragma once
#include "engine/core/Vec3i.h"

class BlockPos : public Vec3i
{
    BlockPos(const int x, const int y, const int z) : Vec3i(x, y, z) {}

    size_t hash() const {
        return ((size_t)getX() * 73856093) ^ ((size_t)getY() * 19349663) ^ ((size_t)getZ() * 83492791);
    }
};
