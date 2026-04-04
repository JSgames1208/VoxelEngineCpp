#pragma once
#include <vector>
#include "engine/world/Chunk.h"
#include "engine/renderer/block/BakedQuad.h"
#include "engine/renderer/block/FaceBakery.h"
#include "engine/texture/TextureAtlas.h"
#include "engine/world/Level.h"
/*
inline bool isSolid(const Chunk& chunk, const ChunkCoord& coord, Level* world,
                    int x, int y, int z)
{
    if (y < 0) return false;
    if (y >= Chunk::SIZEY)
        return true;

    if (x >= 0 && x < Chunk::SIZEX &&
        z >= 0 && z < Chunk::SIZEZ)
    {
        return chunk.get(x, y, z) != BlockType::AIR;
    }

    // Only edges reach here
    int cx = coord.x;
    int cz = coord.z;

    if (x < 0) { cx--; x += Chunk::SIZEX; }
    else if (x >= Chunk::SIZEX) { cx++; x -= Chunk::SIZEX; }

    if (z < 0) { cz--; z += Chunk::SIZEZ; }
    else if (z >= Chunk::SIZEZ) { cz++; z -= Chunk::SIZEZ; }

    Chunk* target = world->getChunkPtr({cx, cz});
    if (!target) return false; // <--- key change

    return target->get(x, y, z) != BlockType::AIR;
}
*/

class ChunkMesher
{
public:
    ChunkMesher(TextureAtlas* atlas)
        : atlas(atlas) { }

    float getAO(bool side1, bool side2, bool corner)
    {
        if (side1 && side2) return 0.0f;
        int occ = side1 + side2 + corner;
        return 1.0f - occ * 0.25f;
    }

	std::unique_ptr<std::vector<BakedQuad>> meshChunk(const Chunk& chunk, const ChunkCoord& coord, Level* world)
	{
		std::vector<BakedQuad> quads;

        Chunk* neighbors[3][3] = {};
        for (int dx = -1; dx <= 1; dx++)
        {
            for (int dz = -1; dz <= 1; dz++)
            {
                neighbors[dx+1][dz+1] = world->getChunkPtr({coord.x + dx, coord.z + dz});
            }
        }

        auto getBlock = [&](int x, int y, int z) -> BlockType
        {
            if (y < 0) return BlockType::AIR;
            if (y >= Chunk::SIZEY) return BlockType::AIR;

            int cx = 1, cz = 1;
            if (x < 0) { cx--; x += Chunk::SIZEX; }
            else if (x >= Chunk::SIZEX) { cx++; x -= Chunk::SIZEX; }

            if (z < 0) { cz--; z += Chunk::SIZEZ; }
            else if (z >= Chunk::SIZEZ) { cz++; z -= Chunk::SIZEZ; }

            Chunk* target = neighbors[cx][cz];
            if (!target) return BlockType::AIR;

            return target->get(x, y, z);
        };

        auto isSolid = [&](int x, int y, int z) -> bool
        {
            return getBlock(x, y, z) != BlockType::AIR;
        };

        for (int x = 0; x < Chunk::SIZEX; x++)
        {
            for (int y = 0; y < Chunk::SIZEY; y++)
            {
                for (int z = 0; z < Chunk::SIZEZ; z++)
                {
                    BlockType type = chunk.get(x, y, z);
                    if (type == BlockType::AIR)
                        continue;

                    vec3 min(x, y, z);
                    vec3 max(x + 1, y + 1, z + 1);

                    // UP (+Y)
                    if (!isSolid(x, y + 1, z))
                    {
                        bool n = isSolid(x, y + 1, z - 1);
                        bool s = isSolid(x, y + 1, z + 1);
                        bool w = isSolid(x - 1, y + 1, z);
                        bool e = isSolid(x + 1, y + 1, z);

                        bool nw = isSolid(x - 1, y + 1, z - 1);
                        bool ne = isSolid(x + 1, y + 1, z - 1);
                        bool sw = isSolid(x - 1, y + 1, z + 1);
                        bool se = isSolid(x + 1, y + 1, z + 1);

                        float ao[4];
                        ao[0] = getAO(w, n, nw);
                        ao[1] = getAO(e, n, ne);
                        ao[2] = getAO(e, s, se);
                        ao[3] = getAO(w, s, sw);

                        quads.push_back(faceBakery.bakeQuad(min, max, Direction::UP, type, ao, atlas));
                    }

                    // DOWN (-Y)
                    if (!isSolid(x, y - 1, z))
                    {
                        bool n = isSolid(x, y - 1, z - 1);
                        bool s = isSolid(x, y - 1, z + 1);
                        bool w = isSolid(x - 1, y - 1, z);
                        bool e = isSolid(x + 1, y - 1, z);

                        bool nw = isSolid(x - 1, y - 1, z - 1);
                        bool ne = isSolid(x + 1, y - 1, z - 1);
                        bool sw = isSolid(x - 1, y - 1, z + 1);
                        bool se = isSolid(x + 1, y - 1, z + 1);

                        float ao[4];
                        ao[0] = getAO(w, s, sw);
                        ao[1] = getAO(e, s, se);
                        ao[2] = getAO(e, n, ne);
                        ao[3] = getAO(w, n, nw);

                        quads.push_back(faceBakery.bakeQuad(min, max, Direction::DOWN, type, ao, atlas));
                    }

                    // NORTH (-Z)
                    if (!isSolid(x, y, z - 1))
                    {
                        bool u = isSolid(x, y + 1, z - 1);
                        bool d = isSolid(x, y - 1, z - 1);
                        bool w = isSolid(x - 1, y, z - 1);
                        bool e = isSolid(x + 1, y, z - 1);

                        bool uw = isSolid(x - 1, y + 1, z - 1);
                        bool ue = isSolid(x + 1, y + 1, z - 1);
                        bool dw = isSolid(x - 1, y - 1, z - 1);
                        bool de = isSolid(x + 1, y - 1, z - 1);

                        float ao[4];
                        ao[0] = getAO(w, d, dw);
                        ao[1] = getAO(e, d, de);
                        ao[2] = getAO(e, u, ue);
                        ao[3] = getAO(w, u, uw);

                        quads.push_back(faceBakery.bakeQuad(min, max, Direction::NORTH, type, ao, atlas));
                    }

                    // SOUTH (+Z)
                    if (!isSolid(x, y, z + 1))
                    {
                        bool u = isSolid(x, y + 1, z + 1);
                        bool d = isSolid(x, y - 1, z + 1);
                        bool w = isSolid(x - 1, y, z + 1);
                        bool e = isSolid(x + 1, y, z + 1);

                        bool uw = isSolid(x - 1, y + 1, z + 1);
                        bool ue = isSolid(x + 1, y + 1, z + 1);
                        bool dw = isSolid(x - 1, y - 1, z + 1);
                        bool de = isSolid(x + 1, y - 1, z + 1);

                        float ao[4];
                        ao[0] = getAO(e, d, de);
                        ao[1] = getAO(w, d, dw);
                        ao[2] = getAO(w, u, uw);
                        ao[3] = getAO(e, u, ue);

                        quads.push_back(faceBakery.bakeQuad(min, max, Direction::SOUTH, type, ao, atlas));
                    }

                    // WEST (-X)
                    if (!isSolid(x - 1, y, z))
                    {
                        bool u = isSolid(x - 1, y + 1, z);
                        bool d = isSolid(x - 1, y - 1, z);
                        bool n = isSolid(x - 1, y, z - 1);
                        bool s = isSolid(x - 1, y, z + 1);

                        bool un = isSolid(x - 1, y + 1, z - 1);
                        bool us = isSolid(x - 1, y + 1, z + 1);
                        bool dn = isSolid(x - 1, y - 1, z - 1);
                        bool ds = isSolid(x - 1, y - 1, z + 1);

                        float ao[4];
                        ao[0] = getAO(s, d, ds);
                        ao[1] = getAO(n, d, dn);
                        ao[2] = getAO(n, u, un);
                        ao[3] = getAO(s, u, us);

                        quads.push_back(faceBakery.bakeQuad(min, max, Direction::WEST, type, ao, atlas));
                    }

                    // EAST (+X)
                    if (!isSolid(x + 1, y, z))
                    {
                        bool u = isSolid(x + 1, y + 1, z);
                        bool d = isSolid(x + 1, y - 1, z);
                        bool n = isSolid(x + 1, y, z - 1);
                        bool s = isSolid(x + 1, y, z + 1);

                        bool un = isSolid(x + 1, y + 1, z - 1);
                        bool us = isSolid(x + 1, y + 1, z + 1);
                        bool dn = isSolid(x + 1, y - 1, z - 1);
                        bool ds = isSolid(x + 1, y - 1, z + 1);

                        float ao[4];
                        ao[0] = getAO(n, d, dn);
                        ao[1] = getAO(s, d, ds);
                        ao[2] = getAO(s, u, us);
                        ao[3] = getAO(n, u, un);

                        quads.push_back(faceBakery.bakeQuad(min, max, Direction::EAST, type, ao, atlas));
                    }
                }
            }
        }

		return std::make_unique<std::vector<BakedQuad>>(quads);
	}
private:
    FaceBakery faceBakery;
    TextureAtlas* atlas;
};