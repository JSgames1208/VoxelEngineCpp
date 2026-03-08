#pragma once
#include <vector>
#include "engine/world/Chunk.h"
#include "engine/renderer/block/BakedQuad.h"
#include "engine/renderer/block/FaceBakery.h"

class ChunkMesher
{
public:
	ChunkMesher() = default;

	FaceBakery faceBakery;

	bool isSolid(const Chunk& chunk, int x, int y, int z)
	{
		if (x < 0 || y < 0 || z < 0 || x >= Chunk::SIZEX || y >= Chunk::SIZEY || z >= Chunk::SIZEZ)
		{
			return false;
		}

		return chunk.get(x, y, z) != BlockType::AIR;
	}

	std::vector<BakedQuad> meshChunk(const Chunk& chunk)
	{
		std::vector<BakedQuad> quads;

		for (int x = 0; x < Chunk::SIZEX; x++)
		{
			for (int y = 0; y < Chunk::SIZEY; y++)
			{
				for (int z = 0; z < Chunk::SIZEZ; z++)
				{
					BlockType type = chunk.get(x, y, z);
					if (type == BlockType::AIR)
					{
						continue;
					}

					vec3 min(x, y, z);
					vec3 max(x + 1, y + 1, z + 1);

					if (!isSolid(chunk, x, y + 1, z))
						quads.push_back(faceBakery.bakeQuad(min, max, Direction::UP, type));

					if (!isSolid(chunk, x, y - 1, z))
						quads.push_back(faceBakery.bakeQuad(min, max, Direction::DOWN, type));

					if (!isSolid(chunk, x, y, z - 1))
						quads.push_back(faceBakery.bakeQuad(min, max, Direction::NORTH, type));

					if (!isSolid(chunk, x, y, z + 1))
						quads.push_back(faceBakery.bakeQuad(min, max, Direction::SOUTH, type));

					if (!isSolid(chunk, x - 1, y, z))
						quads.push_back(faceBakery.bakeQuad(min, max, Direction::WEST, type));

					if (!isSolid(chunk, x + 1, y, z))
						quads.push_back(faceBakery.bakeQuad(min, max, Direction::EAST, type));
				}
			}
		}

		return quads;
	}
};