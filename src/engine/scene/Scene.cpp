#include "engine/scene/Scene.h"
#include "engine/world/Level.h"
#include "engine/gen/decorators/TreeDecorator.h"
#include "engine/gen/decorators/RockClumpDecorator.h"

Scene::Scene()
{
	world = std::make_unique<Level>();

	// 2. Create the atlas
	atlas = std::make_unique<TextureAtlas>();
	if (!atlas->loadFromJSON()) {
		std::cerr << "Failed to load textures!\n";
		return;
	}

	// 3. Create the mesher with the atlas pointer
	mesher = std::make_unique<ChunkMesher>(atlas.get());

	// 4. Threaded mesher gets a pointer to mesher (safe)
	threadedMesher = std::make_unique<ThreadedChunkMesher>(world.get(), mesher.get());

	// 5. Chunk generator
	generator = std::make_unique<ChunkGenerator>(world.get());
	generator->addDecorator(std::make_unique<TreeDecorator>());
	generator->addDecorator(std::make_unique<RockClumpDecorator>());

	std::cout << "loaded generator and mesher" << std::endl;

	// 6. Queue chunks
	int renderDistance = 25;
	totalChunksToGenerate = (2 * renderDistance + 1) * (2 * renderDistance + 1);
	for (int r = 0; r <= renderDistance; r++)
		for (int x = -r; x <= r; x++)
			for (int z = -r; z <= r; z++)
				if (abs(x) == r || abs(z) == r)
					generator->queueChunk({ x, z });

	// 7. Start timing and generation
	startTime = std::chrono::high_resolution_clock::now();
	timingStarted = true;
	generator->start();

	threadedMesher->start();
}

Scene::~Scene()
{
	generator->stop();
	threadedMesher->stop();
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

			std::cout << "Generated all chunks in: " << ms << " ms (" << ms / 1000.0 << " seconds)\n";

			timingStarted = false;
		}

		ChunkCoord coord = chunk->coord;
		world->addChunk(coord, std::move(chunk));

		markChunkDirty(coord);

		markChunkDirty({ coord.x + 1, coord.z });
		markChunkDirty({ coord.x - 1, coord.z });
		markChunkDirty({ coord.x, coord.z + 1 });
		markChunkDirty({ coord.x, coord.z - 1 });

		if (!meshTimingStarted)
		{
			meshStartTime = std::chrono::high_resolution_clock ::now();
			meshTimingStarted = true;
		}

        while (!dirtyQueue.empty()) {
            ChunkCoord dirtyCoord = dirtyQueue.front();
            dirtyQueue.pop();

            Chunk* dirtyChunk = world->getChunkPtr(dirtyCoord);
            if (!dirtyChunk) continue;

			threadedMesher->queueChunk(dirtyCoord);
        }
	}

	int meshesPerFrame = 3;
	for (int i = 0; i < meshesPerFrame && threadedMesher->hasFinishedMeshes(); ++i)
	{
		auto [coord, quads] = threadedMesher->fetchFinishedMesh();
		if (!quads) continue;

		auto t1 = std::chrono::high_resolution_clock::now();
		auto mesh = threadedMesher->createMeshFromQuads(*quads);
		auto t2 = std::chrono::high_resolution_clock::now();
		//std::cout << "mesh from quads took: " << std::chrono::duration<double, std::milli>(t2 - t1).count() << " ms" << std::endl;

		gpuUploadQueue.push({coord, std::move(mesh)});

		Chunk* chunk = world->getChunkPtr(coord);
		if (chunk)
		{
			chunk->isDirty = false;
			chunk->isQueued = false;
		}
	}

	int maxUploadsPerFrame = 100;
	for (int i = 0; i < maxUploadsPerFrame && !gpuUploadQueue.empty(); ++i)
	{
		auto t1 = std::chrono::high_resolution_clock::now();

		auto [coord, mesh] = std::move(gpuUploadQueue.front());
		gpuUploadQueue.pop();

		chunkMeshes[coord] = std::make_unique<ChunkMesh>(std::move(mesh), coord);

		auto t2 = std::chrono::high_resolution_clock::now();
		//std::cout << "uploading took: " << std::chrono::duration<double, std::milli>(t2 - t1).count() << " ms" << std::endl;
	}

	/*
	if (!threadedMesher->hasFinishedMeshes() && meshTimingStarted)
	{
		auto endTime = std::chrono::high_resolution_clock::now();
		double totalMs = std::chrono::duration<double, std::milli>(endTime - meshStartTime).count();
		std::cout << "Total meshing time so far: " << totalMs << " ms" << std::endl;
	}
	*/
}

void Scene::markChunkDirty(const ChunkCoord& coord)
{
	Chunk* chunk = world->getChunkPtr(coord);
	if (!chunk) return;

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