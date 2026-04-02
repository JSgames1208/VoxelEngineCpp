//
// Created by Jasper Soete on 2-4-2026.
//

#pragma once
#include "engine/world/level/heightproviders/HeightProvider.h"

class ConstantHeight : public HeightProvider
{
public:
    ConstantHeight(int height)
        : height(height)
    {}

    int getHeight(int wx, int wz) const override
    {
        return height;
    }
private:
    int height;
};
