#include "engine/scene/Scene.h"
#include "engine/world/World.h"

Scene::Scene()
	: world(std::make_unique<World>())
{
	generator = std::make_unique<ChunkGenerator>(world.get());

	int renderDistance = 50;
	totalChunksToGenerate = (2 * renderDistance + 1) * (2 * renderDistance + 1);

	for (int r = 0; r <= renderDistance; r++)
	{
		for (int x = -r; x <= r; x++)
		{
			for (int z = -r; z <= r; z++)
			{
				if (abs(x) == r || abs(z) == r)
				{
					generator->queueChunk({ x, z });
				}
			}
		}
	}

	startTime = std::chrono::high_resolution_clock::now();
	timingStarted = true;

	generator->start();
}

Scene::~Scene()
{
	generator->stop();
}

void Scene::update(float deltaTime)
{
	while (generator->hasGeneratedChunks()) {
		auto chunk = generator->fetchGeneratedChunk();
		if (!chunk) continue;

		generatedCount++;

		if (generatedCount == totalChunksToGenerate && timingStarted)
		{
			auto endTime = std::chrono::high_resolution_clock::now();
			double ms = std::chrono::duration<double, std::milli>(endTime - startTime).count();

			std::cout << "Generated all chunks in: " << ms << " ms ("
				<< ms / 1000.0 << " seconds)\n";

			timingStarted = false;
		}

		ChunkCoord coord = chunk->coord;
		world->addChunk(coord, std::move(chunk));

		markChunkDirty(coord);

		markChunkDirty({ coord.x + 1, coord.z });
		markChunkDirty({ coord.x - 1, coord.z });
		markChunkDirty({ coord.x, coord.z + 1 });
		markChunkDirty({ coord.x, coord.z - 1 });
	}

	int meshesPerFrame = 1;

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

void Scene::markChunkDirty(const ChunkCoord& coord)
{
	Chunk* chunk = world->getChunkPtr(coord);
	if (!chunk) return;

	if (chunk->isQueued) return;

	chunk->isDirty = true;
	chunk->isQueued = true;

	dirtyQueue.push(coord);
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