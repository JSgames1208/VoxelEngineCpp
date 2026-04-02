//
// Created by Jasper Soete on 2-4-2026.
//

#include "engine/world/level/feature/FeatureGenerator.h"
#include "engine/world/level/feature/FeatureRegion.h"

FeatureRegionCoord FeatureGenerator::getRegion(ChunkCoord &coord)
{
    return FeatureRegionCoord({coord.x / FeatureRegion::SIZEX, coord.z / FeatureRegion::SIZEZ});
}
