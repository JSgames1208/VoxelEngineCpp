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

	Chunk* World::getChunkPtr(ChunkCoord coord)
	{
		auto it = chunks.find(coord);
		if (it == chunks.end())
			return nullptr;

		return it->second.get();
	}

	BlockType getBlock(int worldX, int worldY, int worldZ)
	{
		if (worldY < 0 || worldY >= Chunk::SIZEY)
			return BlockType::AIR;

		int chunkX = floor(float(worldX) / Chunk::SIZEX);
		int chunkZ = floor(float(worldZ) / Chunk::SIZEZ);

		ChunkCoord coord(chunkX, chunkZ);
		Chunk* chunk = getChunkPtr(coord);

		if (!chunk)
			return BlockType::AIR;

		int localX = (worldX % Chunk::SIZEX + Chunk::SIZEX) % Chunk::SIZEX;
		int localZ = (worldZ % Chunk::SIZEZ + Chunk::SIZEZ) % Chunk::SIZEZ;

		return chunk->get(localX, worldY, localZ);
	}

	std::unordered_map<ChunkCoord, std::unique_ptr<Chunk>> chunks;
private:
};