//
// Created by Jasper Soete on 4-4-2026.
//

#pragma once
#include "engine/platform/DisplayData.h"

class GameConfig
{
public:
    GameConfig(DisplayData displayData);
    const DisplayData& getDisplayData() const;
private:
    const DisplayData displayData;
};
