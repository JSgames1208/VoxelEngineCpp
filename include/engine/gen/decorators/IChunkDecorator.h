#pragma once
#include "engine/world/Chunk.h"

class IChunkDecorator 
{
public:
    virtual ~IChunkDecorator() = default;
    virtual void decorate(Chunk& chunk, const ChunkCoord& coord) = 0;
};