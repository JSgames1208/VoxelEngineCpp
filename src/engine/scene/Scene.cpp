#include "engine/scene/Scene.h"
#include "engine/world/World.h"

Scene::Scene()
	: world(std::make_unique<World>())
{
	int renderDistance = 25;
	for (int i = -renderDistance; i <= renderDistance; ++i)
		for (int j = -renderDistance; j <= renderDistance; ++j)
			chunkQueue.push_back(ChunkCoord(i, j));
}

void Scene::update(float deltaTime)
{
	int chunksPerFrame = 1;
	for (int i = 0; i < chunksPerFrame && !chunkQueue.empty(); ++i)
	{
		ChunkCoord coord = chunkQueue.back();
		chunkQueue.pop_back();

		auto chunk = std::make_unique<Chunk>();

		for (int x = 0; x < Chunk::SIZEX; x++)
			for (int y = 0; y < Chunk::SIZEY; y++)
				for (int z = 0; z < Chunk::SIZEZ; z++)
					chunk->set(x, y, z,
						y == 0 ? BlockType::BEDROCK :
						y < 58 ? BlockType::STONE :
						y < 63 ? BlockType::DIRT :
						BlockType::GRASS_BLOCK
					);

		world->addChunk(coord, std::move(chunk));

		auto& chunkPtr = world->getChunk(coord);
		auto quads = mesher.meshChunk(chunkPtr);
		auto mesh = chunkRenderer.createMeshFromQuads(quads);
		chunkMeshes.push_back(std::make_unique<ChunkMesh>(std::move(mesh), coord));
	}
}

void Scene::render(Shader& shader)
{
	for (auto& chunkMesh : chunkMeshes)
	{
		if (!chunkMesh) {
			std::cerr << "Warning: mesh is null!\n";
			continue;
		}

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(
			chunkMesh->coord.x * Chunk::SIZEX,
			0.0f,
			chunkMesh->coord.z * Chunk::SIZEZ
		));
		shader.setMat4("model", model);

		chunkMesh->mesh->draw();
	}
}