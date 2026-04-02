//
// Created by Jasper Soete on 2-4-2026.
//

#pragma once
#include "engine/world/level/feature/Feature.h"
#include "engine/core/AABB.h"

struct FeatureEntry
{
    AABB box;
    Feature* feature;
};
