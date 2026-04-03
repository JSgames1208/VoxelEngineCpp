#pragma once
#include <vector>
#include "engine/world/block/BlockType.h"
#include "engine/core/ChunkCoord.h"

class Chunk
{
public:
	static const int SIZEX = 16;
	static const int SIZEY = 128;
	static const int SIZEZ = 16;
	Chunk(ChunkCoord _coord, int _seed)
		: coord(_coord)
		, seed(_seed)
	{
		blocks.resize(SIZEX * SIZEY * SIZEZ);
	}

	bool isDirty = true;
	bool isQueued = false;

	ChunkCoord coord;

	BlockType get(int x, int y, int z) const
	{
		return blocks[(x * SIZEY + y) * SIZEZ + z];
	}

	BlockType getAtWorldPos(int wx, int wy, int wz) const
	{
		int x = wx % Chunk::SIZEX;
		int z = wz % Chunk::SIZEZ;

		return this->get(x, wy, z);
	}

	void set(int x, int y, int z, BlockType value)
	{
		blocks[(x * SIZEY + y) * SIZEZ + z] = value;
	}

	int getSeed()
	{
		return seed;
	}
	std::vector<BlockType> blocks;
private:
	int seed;
};