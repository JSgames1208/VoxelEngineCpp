//
// Created by Jasper Soete on 2-4-2026.
//

#pragma once
#include "engine/world/block/BlockType.h"
#include "engine/world/level/LevelWriter.h"
#include "engine/core/BlockPos.h"

class Feature
{
public:
    Feature();
protected:
    void setBlock(LevelWriter& level, const BlockPos& pos, const BlockType& blockType);
};
