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
				if (y == 0)
					chunk.set(x, y, z, BlockType::BEDROCK);
				else if (y > 0 && y < 58)
					chunk.set(x, y, z, BlockType::STONE);
				else if (y >= 58 && y < 63)
					chunk.set(x, y, z, BlockType::DIRT);
				else if (y >= 63)
					chunk.set(x, y, z, BlockType::GRASS_BLOCK);
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