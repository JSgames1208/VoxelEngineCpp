#include "engine/scene/Scene.h"

Scene::Scene()
{
	Chunk chunk;

	for (int x = 0; x < Chunk::SIZEX; x++)
	{
		for (int y = 0; y < Chunk::SIZEY; y++)
		{
			for (int z = 0; z < Chunk::SIZEZ; z++)
			{
				if ((x + y + z) % 2 == 1)
				{
					chunk.set(x, y, z, BlockType::SAND);
				}
				else
				{
					chunk.set(x, y, z, BlockType::BEDROCK);
				}
			}
		}
	}

	chunks.push_back(chunk);

	for (auto& c : chunks)
	{
		auto quads = mesher.meshChunk(c);
		auto mesh = chunkRenderer.createMeshFromQuads(quads);
		chunkMeshes.push_back(std::move(mesh));
	}
}

void Scene::update(float deltaTime)
{
	
}

void Scene::render(Shader& shader)
{
	for (auto& mesh : chunkMeshes)
	{
		if (!mesh) {
			std::cerr << "Warning: mesh is null!\n";
			continue;
		}
		mesh->draw();
	}
}