#pragma once
#include "engine/core/ChunkCoord.h"
#include "engine/world/Level.h"
#include "engine/gen/decorators/IChunkDecorator.h"
#include "engine/world/level/heightproviders/NoiseHeightProvider.h"
#include "FastNoiseLite.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <memory>

class ChunkGenerator
{
public:
	ChunkGenerator(Level* world);

	void queueChunk(const ChunkCoord& coord);
	void processChunks(int chunksPerFrame = 1);

	void start();
	void stop();
	bool hasGeneratedChunks();
	std::unique_ptr<Chunk> fetchGeneratedChunk();

	void addDecorator(std::unique_ptr<IChunkDecorator> decorator);

	bool hasDirtyChunks();
	ChunkCoord popDirtyChunk();
private:
	Level* world;
	FastNoiseLite noise;

	std::queue<ChunkCoord> chunkQueue;
	std::mutex queueMutex;

	std::queue<std::unique_ptr<Chunk>> generatedChunks;
	std::mutex generatedMutex;

	std::queue<ChunkCoord> dirtyQueue;
	std::unordered_map<ChunkCoord, bool> chunkDirtyMap;

    std::unique_ptr<NoiseHeightProvider> heightProvider;

	void markChunkDirty(const ChunkCoord& coord);
	
	std::vector<std::thread> workers;
	std::atomic<bool> running = false;

	std::vector<std::unique_ptr<IChunkDecorator>> decorators;
	std::mutex decoratorMutex;

	std::unique_ptr<Chunk> generateChunk(const ChunkCoord& coord);
	void workerLoop();
};