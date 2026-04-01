#pragma once
#include "engine/world/block/BlockType.h"

class LevelSimulatedReader
{
public:
	virtual bool isAir(int wx, int wy, int wz) const = 0;
	virtual BlockType getBlock(int wx, int wy, int wz) const = 0;
};