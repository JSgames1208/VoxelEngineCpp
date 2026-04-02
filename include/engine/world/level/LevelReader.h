#pragma once
#include "engine/world/level/LevelSimulatedReader.h"
#include "engine/world/Level.h"

class LevelReader : public LevelSimulatedReader
{
public:
	LevelReader(Level* w): world(w) {}

	BlockType getBlock(int wx, int wy, int wz) const override
	{
		Chunk* chunk = world->getChunkPtr(ChunkCoord(wx / Chunk::SIZEX, wz / Chunk::SIZEZ));
		if (!chunk) return BlockType::AIR;
		return chunk->getAtWorldPos(wx, wy, wz);
	}

	bool isAir(int wx, int wy, int wz) const override
	{
		return getBlock(wx, wy, wz) == BlockType::AIR;
	}
private:
    Level* world;
};