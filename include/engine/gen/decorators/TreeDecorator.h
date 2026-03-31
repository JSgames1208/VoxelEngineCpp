#pragma once
#include "engine/gen/decorators/IChunkDecorator.h"
#include <random>

class TreeDecorator : public IChunkDecorator
{
public:
    void decorate(Chunk& chunk, const ChunkCoord& coord) override
    {
        std::mt19937 rng(chunk.getSeed() + coord.x * 73856093 + coord.z * 19349663);
        std::uniform_int_distribution<int> dist(0, 999);

        for (int x = 0; x < Chunk::SIZEX; x++)
            for (int z = 0; z < Chunk::SIZEZ; z++)
                for (int y = Chunk::SIZEY - 1; y >= 0; --y)
                    if (chunk.get(x, y, z) == BlockType::GRASS_BLOCK)
                    {
                        if (dist(rng) < 5)
                            spawnTree(chunk, x, y + 1, z);
                        break;
                    }
    }
private:
    void spawnTree(Chunk& chunk, int x, int y, int z)
    {
        auto isValid = [](int val, int max) { return val >= 0 && val < max; };

        // 1. Random height for trunk
        int trunkHeight = 4 + (rand() % 3); // 4-6 blocks high

        // 2. Build trunk
        for (int i = 0; i < trunkHeight; ++i) {
            if (isValid(y + i, Chunk::SIZEY))
                chunk.set(x, y + i, z, BlockType::LOG);
        }

        int top = y + trunkHeight;

        // 3. Build leaves layer
        for (int dx = -2; dx <= 2; ++dx) {
            for (int dy = 0; dy <= 2; ++dy) {
                for (int dz = -2; dz <= 2; ++dz) {
                    // Skip corners for a rounded shape
                    if (abs(dx) == 2 && abs(dz) == 2) continue;

                    int lx = x + dx;
                    int ly = top + dy;
                    int lz = z + dz;

                    if (isValid(lx, Chunk::SIZEX) && isValid(ly, Chunk::SIZEY) && isValid(lz, Chunk::SIZEZ)) {
                        BlockType existing = chunk.get(lx, ly, lz);
                        if (existing == BlockType::AIR) {
                            chunk.set(lx, ly, lz, BlockType::LEAVES);
                        }
                    }
                }
            }
        }

        // 4. Optional: add a small top leaf
        if (isValid(x, Chunk::SIZEX) && isValid(top + 3, Chunk::SIZEY) && isValid(z, Chunk::SIZEZ))
        {
            chunk.set(x, top + 3, z, BlockType::LEAVES);
        }
    }
};