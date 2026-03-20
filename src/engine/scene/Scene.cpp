#include "engine/scene/Scene.h"
#include "engine/world/World.h"

Scene::Scene()
	: world(std::make_unique<World>())
{
	noise.SetSeed(100);
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	noise.SetFrequency(0.01f);

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
			for (int z = 0; z < Chunk::SIZEZ; z++)
			{
				int worldX = coord.x * Chunk::SIZEX + x;
				int worldZ = coord.z * Chunk::SIZEZ + z;

				int height = (int)getHeight(worldX, worldZ);

				for (int y = 0; y < Chunk::SIZEY; y++)
				{
					BlockType block = BlockType::AIR;
					if (y == 0)
						block = BlockType::BEDROCK;
					else if (y < height - 4)
						block = BlockType::STONE;
					else if (y < height)
						block = BlockType::DIRT;
					else if (y == height)
						block = BlockType::GRASS_BLOCK;

					chunk->set(x, y, z, block);
				}
			}


		world->addChunk(coord, std::move(chunk));

		auto& chunkPtr = world->getChunk(coord);
		auto quads = mesher.meshChunk(chunkPtr, coord, world.get());
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

float Scene::getHeight(int x, int z)
{
	float scale = 1.0f;
	float amplitude = 10.0f;
	float baseHeight = 60.0f;

	float noiseValue = noise.GetNoise((float)x * scale, (float)z * scale);

	return baseHeight + noiseValue * amplitude;
}