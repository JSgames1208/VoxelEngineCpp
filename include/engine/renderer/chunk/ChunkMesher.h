#pragma once
#include <vector>
#include "engine/world/Chunk.h"
#include "engine/renderer/block/BakedQuad.h"
#include "engine/renderer/block/FaceBakery.h"
#include "engine/world/World.h"

class ChunkMesher
{
public:
	ChunkMesher() = default;

	FaceBakery faceBakery;

	bool isSolidHybrid(const Chunk& chunk, const ChunkCoord& coord, World* world,
                   int x, int y, int z)
{
    if (y < 0 || y >= Chunk::SIZEY)
        return false;

    if (x < 0)
    {
        Chunk* neighbor = world->getChunkPtr({coord.x - 1, coord.z});
        if (!neighbor) return false;
        return neighbor->get(Chunk::SIZEX - 1, y, z) != BlockType::AIR;
    }
    else if (x >= Chunk::SIZEX)
    {
        Chunk* neighbor = world->getChunkPtr({coord.x + 1, coord.z});
        if (!neighbor) return false;
        return neighbor->get(0, y, z) != BlockType::AIR;
    }

    if (z < 0)
    {
        Chunk* neighbor = world->getChunkPtr({coord.x, coord.z - 1});
        if (!neighbor) return false;
        return neighbor->get(x, y, Chunk::SIZEZ - 1) != BlockType::AIR;
    }
    else if (z >= Chunk::SIZEZ)
    {
        Chunk* neighbor = world->getChunkPtr({coord.x, coord.z + 1});
        if (!neighbor) return false;
        return neighbor->get(x, y, 0) != BlockType::AIR;
    }

    return chunk.get(x, y, z) != BlockType::AIR;
}

	std::vector<BakedQuad> meshChunk(const Chunk& chunk, const ChunkCoord& coord, World* world)
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
                        continue;

                    int worldX = coord.x * Chunk::SIZEX + x;
                    int worldY = y;
                    int worldZ = coord.z * Chunk::SIZEZ + z;

                    vec3 min(x, y, z);
                    vec3 max(x + 1, y + 1, z + 1);

                    if (!isSolidHybrid(chunk, coord, world, x, y + 1, z))
                    quads.push_back(faceBakery.bakeQuad(min, max, Direction::UP, type));

                    // DOWN (-Y)
                    if (!isSolidHybrid(chunk, coord, world, x, y - 1, z))
                        quads.push_back(faceBakery.bakeQuad(min, max, Direction::DOWN, type));

                    // NORTH (-Z)
                    if (!isSolidHybrid(chunk, coord, world, x, y, z - 1))
                        quads.push_back(faceBakery.bakeQuad(min, max, Direction::NORTH, type));

                    // SOUTH (+Z)
                    if (!isSolidHybrid(chunk, coord, world, x, y, z + 1))
                        quads.push_back(faceBakery.bakeQuad(min, max, Direction::SOUTH, type));

                    // WEST (-X)
                    if (!isSolidHybrid(chunk, coord, world, x - 1, y, z))
                        quads.push_back(faceBakery.bakeQuad(min, max, Direction::WEST, type));

                    // EAST (+X)
                    if (!isSolidHybrid(chunk, coord, world, x + 1, y, z))
                        quads.push_back(faceBakery.bakeQuad(min, max, Direction::EAST, type));
                }
            }
        }

		return quads;
	}
};