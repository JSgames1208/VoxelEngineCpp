#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include "engine/Shader.h"
#include "engine/world/Chunk.h"
#include "engine/renderer/chunk/ChunkMesher.h"
#include "engine/renderer/chunk/ChunkRenderer.h"
#include "engine/mesh/Mesh.h"

class Scene
{
public:
	Scene();

	void update(float deltaTime);
	void render(Shader& shader);

private:
	std::vector<Chunk> chunks;
	std::vector<std::unique_ptr<Mesh>> chunkMeshes;
	ChunkMesher mesher;
	ChunkRenderer chunkRenderer;
};