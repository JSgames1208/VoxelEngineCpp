#include "engine/mesh/ThreadedChunkMesher.h"
#include <iostream>
#include <chrono>

ThreadedChunkMesher::ThreadedChunkMesher(Level* world, ChunkMesher* mesher)
	: world(world)
	, mesher(mesher)
{
}

void ThreadedChunkMesher::queueChunk(const ChunkCoord& coord)
{
	{
		std::lock_guard<std::mutex> lock(queueMutex);
		meshingQueue.push(coord);
	}
	cv.notify_one();
}

void ThreadedChunkMesher::start(int threadCount)
{
	running = true;
	threadCount = std::min((unsigned int)threadCount, std::thread::hardware_concurrency());
	for (int i = 0; i < threadCount; i++)
	{
		workers.emplace_back(std::thread(&ThreadedChunkMesher::workerLoop, this));
	}
}

void ThreadedChunkMesher::stop()
{
	{
		std::lock_guard<std::mutex> lock(queueMutex);
		running = false;
	}
	cv.notify_all();
	for (auto& t : workers)
	{
		if (t.joinable()) t.join();
	}
}

bool ThreadedChunkMesher::hasFullNeighborhood(Level *world, const ChunkCoord &coord)
{
	for (int dx = -1; dx <= 1; dx++)
	{
		for (int dz = -1; dz <= 1; dz++)
		{
			if (!world->getChunkPtr({coord.x + dx, coord.z + dz}))
				return false;
		}
	}
	return true;
}

std::unique_ptr<MeshData> ThreadedChunkMesher::createMeshFromQuads(const std::vector<BakedQuad>& quads)
{
    auto meshData = std::make_unique<MeshData>();
	auto& vertices = meshData->vertices;
	auto& indices = meshData->indices;

	size_t quadCount = quads.size();
	vertices.resize(quadCount * 4 * 9);
	indices.resize(quadCount * 6);

	float* vPtr = vertices.data();
	uint32_t* iPtr = indices.data();
	unsigned int indexOffset = 0;

	for (const auto& quad : quads)
	{
		for (int i = 0; i < 4; i++)
		{
			auto pos = quad.getPosition(i);
			auto uv = quad.getUV(i);
			auto ao = quad.getAO(i);

			*vPtr++ = pos.x;
			*vPtr++ = pos.y;
			*vPtr++ = pos.z;

			*vPtr++ = Directions::get(quad.direction).vector.x;
			*vPtr++ = Directions::get(quad.direction).vector.y;
			*vPtr++ = Directions::get(quad.direction).vector.z;

			*vPtr++ = uv.x;
			*vPtr++ = uv.y;

			*vPtr++ = ao;
		}

		*iPtr++ = indexOffset;
		*iPtr++ = indexOffset + 2;
		*iPtr++ = indexOffset + 1;

		*iPtr++ = indexOffset;
		*iPtr++ = indexOffset + 3;
		*iPtr++ = indexOffset + 2;

		indexOffset += 4;
	}

	return meshData;
}

void ThreadedChunkMesher::workerLoop()
{
	while (running)
	{
		ChunkCoord coord;
		{
			std::unique_lock<std::mutex> lock(queueMutex);
			cv.wait(lock, [&]{ return !meshingQueue.empty() || !running; });

			if (!running && meshingQueue.empty())
			{
				return;
			}
			coord = meshingQueue.front();
			meshingQueue.pop();
		}

		Chunk* chunk = world->getChunkPtr(coord);
		if (!chunk) continue;

		auto t0 = std::chrono::high_resolution_clock::now();
		auto quads = mesher->meshChunk(*chunk, coord, world);
		auto t1 = std::chrono::high_resolution_clock::now();

		//std::cout << "Chunk meshing took: "
		//		  << std::chrono::duration<double, std::milli>(t1-t0).count()
		//		  << " ms\n";


		{
			std::lock_guard<std::mutex> lock(finishedMutex);
			finishedMeshes.push({ coord, std::move(quads)});
		}
	}
}

bool ThreadedChunkMesher::hasFinishedMeshes()
{
	std::lock_guard<std::mutex> lock(finishedMutex);
	return !finishedMeshes.empty();
}

std::pair<ChunkCoord, std::unique_ptr<std::vector<BakedQuad>>> ThreadedChunkMesher::fetchFinishedMesh()
{
	std::lock_guard<std::mutex> lock(finishedMutex);
	if (finishedMeshes.empty()) return { ChunkCoord(0, 0), nullptr};

	auto mesh = std::move(finishedMeshes.front());
	finishedMeshes.pop();
	return mesh;
}