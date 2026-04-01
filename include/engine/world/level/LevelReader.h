#pragma once
#include "engine/world/level/LevelSimulatedReader.h"
#include "engine/world/World.h"

class LevelReader : public LevelSimulatedReader
{
public:
	LevelReader(World* w): world(w) {}

	BlockType getBlock(int wx, int wy, int wz) const override
	{
		Chunk chunk = world->getChunkPtr(wx / Chunk::SIZEX, wz / Chunk::SIZEZ);
		if (!chunk) return BlockType::AIR;
		return chunk.get(wx % SIZEX, wy, wz % Chunk::SIZEZ)
	}

	bool isAir(int wx, int wy, int wz) const override
	{
		return getBlock(wx, wy, wz) == BlockType::AIR;
	}
private:
	World* world;
};