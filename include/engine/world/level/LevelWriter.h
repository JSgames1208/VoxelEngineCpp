//
// Created by Jasper Soete on 2-4-2026.
//

#pragma once

#include "engine/core/BlockPos.h"
#include "engine/world/block/BlockType.h"

class LevelWriter
{
public:
    bool setBlock(const BlockPos pos, const BlockType blockType) { return false; };
    bool removeBlock(const BlockPos pos) { return false; };
};
