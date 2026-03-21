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

    float getAO(bool side1, bool side2, bool corner)
    {
        if (side1 && side2) return 0.0f;
        int occ = side1 + side2 + corner;
        return 1.0f - occ * 0.25f;
    }

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

                    // UP (+Y)
                    if (!isSolidHybrid(chunk, coord, world, x, y + 1, z))
                    {
                        // Only check blocks above this face
                        bool n = isSolidHybrid(chunk, coord, world, x, y + 1, z - 1);
                        bool s = isSolidHybrid(chunk, coord, world, x, y + 1, z + 1);
                        bool w = isSolidHybrid(chunk, coord, world, x - 1, y + 1, z);
                        bool e = isSolidHybrid(chunk, coord, world, x + 1, y + 1, z);

                        bool nw = isSolidHybrid(chunk, coord, world, x - 1, y + 1, z - 1);
                        bool ne = isSolidHybrid(chunk, coord, world, x + 1, y + 1, z - 1);
                        bool sw = isSolidHybrid(chunk, coord, world, x - 1, y + 1, z + 1);
                        bool se = isSolidHybrid(chunk, coord, world, x + 1, y + 1, z + 1);

                        float ao[4];
                        ao[0] = getAO(w, n, nw);
                        ao[1] = getAO(e, n, ne);
                        ao[2] = getAO(e, s, se);
                        ao[3] = getAO(w, s, sw);

                        quads.push_back(faceBakery.bakeQuad(min, max, Direction::UP, type, ao));
                    }

                    // DOWN (-Y)
                    if (!isSolidHybrid(chunk, coord, world, x, y - 1, z))
                    {
                        // Only check blocks below
                        bool n = isSolidHybrid(chunk, coord, world, x, y - 1, z - 1);
                        bool s = isSolidHybrid(chunk, coord, world, x, y - 1, z + 1);
                        bool w = isSolidHybrid(chunk, coord, world, x - 1, y - 1, z);
                        bool e = isSolidHybrid(chunk, coord, world, x + 1, y - 1, z);

                        bool nw = isSolidHybrid(chunk, coord, world, x - 1, y - 1, z - 1);
                        bool ne = isSolidHybrid(chunk, coord, world, x + 1, y - 1, z - 1);
                        bool sw = isSolidHybrid(chunk, coord, world, x - 1, y - 1, z + 1);
                        bool se = isSolidHybrid(chunk, coord, world, x + 1, y - 1, z + 1);

                        float ao[4];
                        ao[0] = getAO(w, s, sw);
                        ao[1] = getAO(e, s, se);
                        ao[2] = getAO(e, n, ne);
                        ao[3] = getAO(w, n, nw);

                        quads.push_back(faceBakery.bakeQuad(min, max, Direction::DOWN, type, ao));
                    }

                    // NORTH (-Z)
                    if (!isSolidHybrid(chunk, coord, world, x, y, z - 1))
                    {
                        // Only check neighbors in -Z direction
                        bool u = isSolidHybrid(chunk, coord, world, x, y + 1, z - 1);
                        bool d = isSolidHybrid(chunk, coord, world, x, y - 1, z - 1);
                        bool w = isSolidHybrid(chunk, coord, world, x - 1, y, z - 1);
                        bool e = isSolidHybrid(chunk, coord, world, x + 1, y, z - 1);

                        bool uw = isSolidHybrid(chunk, coord, world, x - 1, y + 1, z - 1);
                        bool ue = isSolidHybrid(chunk, coord, world, x + 1, y + 1, z - 1);
                        bool dw = isSolidHybrid(chunk, coord, world, x - 1, y - 1, z - 1);
                        bool de = isSolidHybrid(chunk, coord, world, x + 1, y - 1, z - 1);

                        float ao[4];
                        ao[0] = getAO(w, d, dw);
                        ao[1] = getAO(e, d, de);
                        ao[2] = getAO(e, u, ue);
                        ao[3] = getAO(w, u, uw);

                        quads.push_back(faceBakery.bakeQuad(min, max, Direction::NORTH, type, ao));
                    }

                    // SOUTH (+Z)
                    if (!isSolidHybrid(chunk, coord, world, x, y, z + 1))
                    {
                        bool u = isSolidHybrid(chunk, coord, world, x, y + 1, z + 1);
                        bool d = isSolidHybrid(chunk, coord, world, x, y - 1, z + 1);
                        bool w = isSolidHybrid(chunk, coord, world, x - 1, y, z + 1);
                        bool e = isSolidHybrid(chunk, coord, world, x + 1, y, z + 1);

                        bool uw = isSolidHybrid(chunk, coord, world, x - 1, y + 1, z + 1);
                        bool ue = isSolidHybrid(chunk, coord, world, x + 1, y + 1, z + 1);
                        bool dw = isSolidHybrid(chunk, coord, world, x - 1, y - 1, z + 1);
                        bool de = isSolidHybrid(chunk, coord, world, x + 1, y - 1, z + 1);

                        float ao[4];
                        ao[0] = getAO(e, d, de);
                        ao[1] = getAO(w, d, dw);
                        ao[2] = getAO(w, u, uw);
                        ao[3] = getAO(e, u, ue);

                        quads.push_back(faceBakery.bakeQuad(min, max, Direction::SOUTH, type, ao));
                    }

                    // WEST (-X)
                    if (!isSolidHybrid(chunk, coord, world, x - 1, y, z))
                    {
                        bool u = isSolidHybrid(chunk, coord, world, x - 1, y + 1, z);
                        bool d = isSolidHybrid(chunk, coord, world, x - 1, y - 1, z);
                        bool n = isSolidHybrid(chunk, coord, world, x - 1, y, z - 1);
                        bool s = isSolidHybrid(chunk, coord, world, x - 1, y, z + 1);

                        bool un = isSolidHybrid(chunk, coord, world, x - 1, y + 1, z - 1);
                        bool us = isSolidHybrid(chunk, coord, world, x - 1, y + 1, z + 1);
                        bool dn = isSolidHybrid(chunk, coord, world, x - 1, y - 1, z - 1);
                        bool ds = isSolidHybrid(chunk, coord, world, x - 1, y - 1, z + 1);

                        float ao[4];
                        ao[0] = getAO(s, d, ds);
                        ao[1] = getAO(n, d, dn);
                        ao[2] = getAO(n, u, un);
                        ao[3] = getAO(s, u, us);

                        quads.push_back(faceBakery.bakeQuad(min, max, Direction::WEST, type, ao));
                    }

                    // EAST (+X)
                    if (!isSolidHybrid(chunk, coord, world, x + 1, y, z))
                    {
                        bool u = isSolidHybrid(chunk, coord, world, x + 1, y + 1, z);
                        bool d = isSolidHybrid(chunk, coord, world, x + 1, y - 1, z);
                        bool n = isSolidHybrid(chunk, coord, world, x + 1, y, z - 1);
                        bool s = isSolidHybrid(chunk, coord, world, x + 1, y, z + 1);

                        bool un = isSolidHybrid(chunk, coord, world, x + 1, y + 1, z - 1);
                        bool us = isSolidHybrid(chunk, coord, world, x + 1, y + 1, z + 1);
                        bool dn = isSolidHybrid(chunk, coord, world, x + 1, y - 1, z - 1);
                        bool ds = isSolidHybrid(chunk, coord, world, x + 1, y - 1, z + 1);

                        float ao[4];
                        ao[0] = getAO(n, d, dn);
                        ao[1] = getAO(s, d, ds);
                        ao[2] = getAO(s, u, us);
                        ao[3] = getAO(n, u, un);

                        quads.push_back(faceBakery.bakeQuad(min, max, Direction::EAST, type, ao));
                    }
                }
            }
        }

		return quads;
	}
};