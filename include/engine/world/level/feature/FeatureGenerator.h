//
// Created by Jasper Soete on 2-4-2026.
//

#pragma once
#include "engine/core/FeatureRegionCoord.h"
#include "engine/core/ChunkCoord.h"
#include "engine/world/level/feature/FeatureEntry.h"

class FeatureGenerator
{
public:
    FeatureGenerator() = default;

    FeatureRegionCoord getRegion(ChunkCoord& coord);

    std::vector<FeatureEntry> getFeaturesForRegion(FeatureRegionCoord coord);
};