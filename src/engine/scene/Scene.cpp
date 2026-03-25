#include "engine/scene/Scene.h"
#include "engine/world/World.h"

Scene::Scene()
	: world(std::make_unique<World>())
{
	noise.SetSeed(100);
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	noise.SetFrequency(0.01f);

	int renderDistance = 10;
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

		// mark self
		markChunkDirty(coord);

		// mark neighbors
		markChunkDirty({ coord.x + 1, coord.z });
		markChunkDirty({ coord.x - 1, coord.z });
		markChunkDirty({ coord.x, coord.z + 1 });
		markChunkDirty({ coord.x, coord.z - 1 });
	}

	int meshesPerFrame = 2;

	for (int i = 0; i < meshesPerFrame && !dirtyQueue.empty(); ++i)
	{
		ChunkCoord coord = dirtyQueue.front();
		dirtyQueue.pop();

		Chunk* chunk = world->getChunkPtr(coord);
		if (!chunk) continue;

		auto quads = mesher.meshChunk(*chunk, coord, world.get());
		std::unique_ptr<Mesh> mesh = chunkRenderer.createMeshFromQuads(quads);

		updateChunkMesh(coord, std::move(mesh));

		chunk->isDirty = false;
		chunk->isQueued = false;
	}
}

void Scene::updateChunkMesh(const ChunkCoord& coord, std::unique_ptr<Mesh> mesh)
{
	chunkMeshes[coord] = std::make_unique<ChunkMesh>(std::move(mesh), coord);
}

void Scene::render(Shader& shader)
{
	for (auto& [coord, chunkMesh] : chunkMeshes)
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

void Scene::markChunkDirty(const ChunkCoord& coord)
{
	Chunk* chunk = world->getChunkPtr(coord);
	if (!chunk) return;

	if (chunk->isQueued) return;

	chunk->isDirty = true;
	chunk->isQueued = true;

	dirtyQueue.push(coord);
}

float Scene::getHeight(int x, int z)
{
	float baseHeight = 64.0f;

	// Big chaotic terrain
	float scale1 = 0.12f;       // large-scale features
	float amplitude1 = 30.0f;  // tall peaks
	float noise1 = noise.GetNoise(x * scale1, z * scale1);

	// Secondary medium bumps
	float scale2 = 0.2f;        // slightly higher frequency
	float amplitude2 = 15.0f;
	float noise2 = noise.GetNoise((x + 9876) * scale2, (z - 5432) * scale2);

	// Fine details / jaggedness
	float scale3 = 0.8f;        // very high frequency
	float amplitude3 = 10.0f;
	float noise3 = noise.GetNoise((x - 13579) * scale3, (z + 24680) * scale3);

	float scale4 = 2.5f;
	float amplitude4 = 5.0f;
	float noise4 = noise.GetNoise((x - 18346) * scale4, (z + 123624) * scale4);

	float scale5 = 4.0f;
	float amplitude5 = 2.0f;
	float noise5 = noise.GetNoise((x - 26729) * scale5, (z + 62365) * scale5);

	// Combine everything
	return baseHeight + noise1 * amplitude1 + noise2 * amplitude2 + noise3 * amplitude3 + noise4 * amplitude4 + noise5 * amplitude5;
}