#pragma once
#include "engine/gen/decorators/IChunkDecorator.h"
#include <random>

class RockClumpDecorator : public IChunkDecorator
{
public:
	void decorate(Chunk& chunk, const ChunkCoord& coord) override
	{
        std::mt19937 rng(
            chunk.getSeed()       
            + coord.x * 374761393
            + coord.z * 668265263
            + 246813579           
        );
        std::uniform_int_distribution<int> dist(0, 999);

        for (int x = 0; x < Chunk::SIZEX; x++)
        {
            for (int z = 0; z < Chunk::SIZEZ; z++)
            {
                for (int y = Chunk::SIZEY - 1; y >= 0; --y)
                {
                    if (chunk.get(x, y, z) == BlockType::GRASS_BLOCK)
                    {
                        if (dist(rng) < 1)
                        {
                            spawnRockClump(chunk, x, y + 1, z, rng);
                        }
                        break;
                    }
                }
            }
        }
	}
private:

    //CHATGPT, AS DEMO
    void spawnRockClump(Chunk& chunk, int x, int y, int z, std::mt19937& rng)
    {
        auto isValid = [](int val, int max) { return val >= 0 && val < max; };
        std::uniform_int_distribution<int> sizeDist(2, 3);
        std::uniform_int_distribution<int> heightDist(1, 2);
        std::uniform_int_distribution<int> blockDist(0, 1); // 0 = COBBLESTONE, 1 = STONE

        int width = sizeDist(rng);
        int depth = sizeDist(rng);
        int height = heightDist(rng);

        for (int dx = 0; dx < width; ++dx)
        {
            for (int dz = 0; dz < depth; ++dz)
            {
                int nx = x + dx;
                int nz = z + dz;

                if (!isValid(nx, Chunk::SIZEX) || !isValid(nz, Chunk::SIZEZ))
                    continue;

                // Small bumpiness for height
                for (int dy = 0; dy < height + (dx + dz) % 2; ++dy)
                {
                    int ny = y + dy;
                    if (!isValid(ny, Chunk::SIZEY))
                        continue;

                    // Pick stone or cobblestone randomly
                    BlockType block = blockDist(rng) == 0 ? BlockType::COBBLESTONE : BlockType::STONE;
                    chunk.set(nx, ny - 1, nz, block);
                }
            }
        }
    }
};