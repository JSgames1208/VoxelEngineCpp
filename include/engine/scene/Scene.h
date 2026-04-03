#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <glm/gtc/matrix_transform.hpp>
#include "engine/Shader.h"
#include "engine/world/Chunk.h"
#include "engine/mesh/ChunkMesher.h"
#include "engine/renderer/chunk/ChunkRenderer.h"
#include "engine/mesh/Mesh.h"
#include "engine/world/Level.h"
#include "engine/core/ChunkCoord.h"
#include "engine/gen/ChunkGenerator.h"
#include "engine/mesh/ThreadedChunkMesher.h"
#include <chrono>

struct PendingChunk {
	PendingChunk(const ChunkCoord& _coord, std::unique_ptr<Chunk> _chunk)
		: chunk(std::move(_chunk)), coord(_coord)
	{
	}

	ChunkCoord coord;
	std::unique_ptr<Chunk> chunk;
};

struct ChunkMesh
{
	ChunkMesh(std::unique_ptr<Mesh> _mesh, const ChunkCoord& _coord)
		: mesh(std::move(_mesh)), coord(_coord)
	{
	}

	std::unique_ptr<Mesh> mesh;
	ChunkCoord coord;
};

class Scene
{
public:
	Scene();
	~Scene();

	void update(float deltaTime);
	void render(Shader& shader);
private:
	void markChunkDirty(const ChunkCoord& coord);
	void updateChunkMesh(const ChunkCoord& coord, std::unique_ptr<Mesh> mesh);
private:
	std::unique_ptr<ChunkMesher> mesher;
	std::vector<Chunk> chunks;
	std::unordered_map<ChunkCoord, std::unique_ptr<ChunkMesh>> chunkMeshes;
	std::unique_ptr<Level> world;
	std::unique_ptr<ChunkGenerator> generator;
	std::unique_ptr<ThreadedChunkMesher> threadedMesher;
	std::queue<ChunkCoord> dirtyQueue;
	std::unique_ptr<TextureAtlas> atlas;
	ChunkRenderer chunkRenderer;

	int totalChunksToGenerate = 0;
	int generatedCount = 0;
	bool timingStarted = false;

	bool meshTimingStarted = false;

	std::chrono::high_resolution_clock::time_point startTime;
	std::chrono::high_resolution_clock::time_point meshStartTime;
};