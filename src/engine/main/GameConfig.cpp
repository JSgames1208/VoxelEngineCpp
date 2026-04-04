//
// Created by Jasper Soete on 4-4-2026.
//

#include "engine/main/GameConfig.h"

GameConfig::GameConfig(DisplayData displayData)
    : displayData(displayData)
{}

const DisplayData &GameConfig::getDisplayData() const
{
    return displayData;
}