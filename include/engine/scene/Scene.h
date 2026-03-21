#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <glm/gtc/matrix_transform.hpp>
#include "engine/Shader.h"
#include "engine/world/Chunk.h"
#include "engine/renderer/chunk/ChunkMesher.h"
#include "engine/renderer/chunk/ChunkRenderer.h"
#include "engine/mesh/Mesh.h"
#include "engine/world/World.h"
#include "engine/core/ChunkCoord.h"
#include "FastNoiseLite.h"

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

	void update(float deltaTime);
	void render(Shader& shader);

	void markChunkDirty(const ChunkCoord& coord);
private:
	float getHeight(int x, int z);
	void updateChunkMesh(const ChunkCoord& coord, std::unique_ptr<Mesh> mesh);

private:
	std::vector<Chunk> chunks;
	std::unordered_map<ChunkCoord, std::unique_ptr<ChunkMesh>> chunkMeshes;
	std::vector<ChunkCoord> chunkQueue;
	std::unique_ptr<World> world;
	std::queue<ChunkCoord> dirtyQueue;
	FastNoiseLite noise;
	ChunkMesher mesher;
	ChunkRenderer chunkRenderer;
};