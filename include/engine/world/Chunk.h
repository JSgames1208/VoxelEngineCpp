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
	}

	bool isDirty = true;
	bool isQueued = false;


	ChunkCoord coord;

	BlockType blocks[SIZEX][SIZEY][SIZEZ];

	BlockType get(int x, int y, int z) const
	{
		return blocks[x][y][z];
	}

	BlockType getAtWorldPos(int wx, int wy, int wz) const
	{
		return blocks[wx % Chunk::SIZEX][wy][wz % Chunk::SIZEZ];
	}

	void set(int x, int y, int z, BlockType value)
	{
		blocks[x][y][z] = value;
	}

	int getSeed()
	{
		return seed;
	}
private:
	int seed;
};