#pragma once
#include "engine/mesh/ChunkMesher.h"
#include "engine/world/Level.h"
#include "engine/mesh/Mesh.h"
#include <mutex>
#include <queue>
#include <memory>
#include <thread>
#include <vector>
#include <atomic>

class ThreadedChunkMesher
{
public:
	ThreadedChunkMesher(Level* world, ChunkMesher* mesher);
	~ThreadedChunkMesher() = default;

	void queueChunk(const ChunkCoord& coord);
	void start(int numThreads = 8);
	void stop();

	bool hasFullNeighborhood(Level* world, const ChunkCoord& coord);

	bool hasFinishedMeshes();
	std::pair<ChunkCoord, std::unique_ptr<std::vector<BakedQuad>>> fetchFinishedMesh();

	std::unique_ptr<Mesh> createMeshFromQuads(const std::vector<BakedQuad>& quads);
private:
	Level* world;
	ChunkMesher* mesher;

	std::queue<ChunkCoord> meshingQueue;
	std::mutex queueMutex;

	std::queue<std::pair<ChunkCoord, std::unique_ptr<std::vector<BakedQuad>>>> finishedMeshes;
	std::mutex finishedMutex;

	std::vector<std::thread> workers;
	std::atomic<bool> running = false;

	void workerLoop();
};