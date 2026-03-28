#pragma once
#include "engine/core/ChunkCoord.h"
#include "engine/world/World.h"
#include "FastNoiseLite.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <memory>

class ChunkGenerator
{
public:
	ChunkGenerator(World* world);

	void queueChunk(const ChunkCoord& coord);
	void processChunks(int chunksPerFrame = 1);

	void start();
	void stop();
	bool hasGeneratedChunks();
	std::unique_ptr<Chunk> fetchGeneratedChunk();

	bool hasDirtyChunks();
	ChunkCoord popDirtyChunk();
private:
	World* world;
	FastNoiseLite noise;

	std::queue<ChunkCoord> chunkQueue;
	std::mutex queueMutex;

	std::queue<std::unique_ptr<Chunk>> generatedChunks;
	std::mutex generatedMutex;

	std::queue<ChunkCoord> dirtyQueue;
	std::unordered_map<ChunkCoord, bool> chunkDirtyMap;


	void markChunkDirty(const ChunkCoord& coord);
	
	std::thread worker;
	std::atomic<bool> running = false;

	std::unique_ptr<Chunk> generateChunk(const ChunkCoord& coord);
	void workerLoop();
};