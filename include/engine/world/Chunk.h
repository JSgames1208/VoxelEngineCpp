#pragma once
#include <vector>
#include "engine/world/block/BlockType.h"

class Chunk
{
public:
	static const int SIZEX = 16;
	static const int SIZEY = 64;
	static const int SIZEZ = 16;
	Chunk() = default;

	BlockType blocks[SIZEX][SIZEY][SIZEZ];

	BlockType get(int x, int y, int z) const
	{
		return blocks[x][y][z];
	}

	void set(int x, int y, int z, BlockType value)
	{
		blocks[x][y][z] = value;
	}
private:
};