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
	std::lock_guard<std::mutex> lock(queueMutex);
	meshingQueue.push(coord);
}

void ThreadedChunkMesher::start(int threadCount)
{
	threadCount = std::min((unsigned int)threadCount, std::thread::hardware_concurrency());
	running = true;
	for (int i = 0; i < threadCount; i++)
	{
		workers.emplace_back(std::thread(&ThreadedChunkMesher::workerLoop, this));
	}
}

void ThreadedChunkMesher::stop()
{
	running = false;
	for (auto& t : workers)
	{
		if (t.joinable()) t.join();
	}
}

std::unique_ptr<Mesh> ThreadedChunkMesher::createMeshFromQuads(const std::vector<BakedQuad>& quads)
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	unsigned int indexOffset = 0;

	for (const auto& quad : quads)
	{
		for (int i = 0; i < 4; i++)
		{
			auto pos = quad.getPosition(i);
			auto uv = quad.getUV(i);
			auto ao = quad.getAO(i);

			vertices.push_back(pos.x);
			vertices.push_back(pos.y);
			vertices.push_back(pos.z);

			vertices.push_back(Directions::get(quad.direction).vector.x);
			vertices.push_back(Directions::get(quad.direction).vector.y);
			vertices.push_back(Directions::get(quad.direction).vector.z);

			vertices.push_back(uv.x);
			vertices.push_back(uv.y);

			vertices.push_back(ao);
		}

		indices.push_back(indexOffset);
		indices.push_back(indexOffset + 2);
		indices.push_back(indexOffset + 1);

		indices.push_back(indexOffset);
		indices.push_back(indexOffset + 3);
		indices.push_back(indexOffset + 2);

		indexOffset += 4;
	}

	auto mesh = std::make_unique<Mesh>();

	auto start = std::chrono::high_resolution_clock::now();

	mesh->setData(vertices, indices);

	auto end = std::chrono::high_resolution_clock::now();
	double ms = std::chrono::duration<double, std::milli>(end - start).count();

	return mesh;
}

void ThreadedChunkMesher::workerLoop()
{
	while (running)
	{
		ChunkCoord coord;

		{
			std::lock_guard<std::mutex> lock(queueMutex);
			if (meshingQueue.empty())
			{
				continue;
			}
			coord = meshingQueue.front();
			meshingQueue.pop();
		}

		Chunk* chunk = world->getChunkPtr(coord);
		if (!chunk) continue;

		auto quads = mesher->meshChunk(*chunk, coord, world);

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