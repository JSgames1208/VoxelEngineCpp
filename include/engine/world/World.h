#pragma once
#include <unordered_map>
#include <memory>
#include "engine/core/ChunkCoord.h"
#include "engine/world/Chunk.h"

class World
{
public:
	World() = default;

	void addChunk(ChunkCoord& coord, std::unique_ptr<Chunk> chunk)
	{
		chunks.emplace(coord, std::move(chunk));
	}

	Chunk& getChunk(ChunkCoord& coord)
	{
		return *chunks.at(coord);
	}

	std::unordered_map<ChunkCoord, std::unique_ptr<Chunk>> chunks;
private:
};