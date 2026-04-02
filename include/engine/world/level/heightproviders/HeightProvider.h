//
// Created by Jasper Soete on 2-4-2026.
//

#pragma once

class HeightProvider
{
public:
    HeightProvider() = default;
    virtual int getHeight(int wx, int wz) const { return 0; };
};
