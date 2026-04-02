//
// Created by Jasper Soete on 2-4-2026.
//

#include "engine/world/level/feature/Feature.h"

Feature::Feature() = default;

void Feature::setBlock(LevelWriter& level, const BlockPos &pos, const BlockType &blockType)
{
    level.setBlock(pos, blockType);
}