//
// Created by Jasper Soete on 2-4-2026.
//

#pragma once
#include "engine/world/level/heightproviders/HeightProvider.h"
#include "FastNoiseLite.h"

class NoiseHeightProvider : public HeightProvider
{
public:
    NoiseHeightProvider(const FastNoiseLite& noise)
        : noise(noise)
    {}

    int getHeight(int wx, int wz) const override
    {
        return static_cast<int>(64 + noise.GetNoise(wx * 0.5f, wz * 0.5f) * 20);
    }
private:
    FastNoiseLite noise;
};
