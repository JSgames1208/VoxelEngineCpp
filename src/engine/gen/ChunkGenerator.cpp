#include "engine/gen/ChunkGenerator.h"

ChunkGenerator::ChunkGenerator(Level* world)
	: world(world)
{
	noise.SetSeed(100);
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	noise.SetFrequency(0.01f);
	heightProvider = std::make_unique<NoiseHeightProvider>(noise);
}

void ChunkGenerator::queueChunk(const ChunkCoord& coord)
{
	chunkQueue.push(coord);
}

void ChunkGenerator::start()
{
	int threadCount = std::min(4u, std::thread::hardware_concurrency());
	running = true;
	for (int i = 0; i < threadCount; i++)
	{
		workers.emplace_back(std::thread(&ChunkGenerator::workerLoop, this));
	}
}

void ChunkGenerator::addDecorator(std::unique_ptr<IChunkDecorator> decorator)
{
	decorators.push_back(std::move(decorator));
}

void ChunkGenerator::stop()
{
	running = false;
	for (auto& t : workers)
	{
		if (t.joinable()) t.join();
	}
}

void ChunkGenerator::workerLoop()
{
	while (running) 
	{
		ChunkCoord coord;

		{
			std::lock_guard<std::mutex> lock(queueMutex);
			if (chunkQueue.empty())
			{
				continue;
			}
			coord = chunkQueue.front();
			chunkQueue.pop();
		}

		auto chunk = generateChunk(coord);

		{
			std::lock_guard<std::mutex> lock(generatedMutex);
			generatedChunks.push(std::move(chunk));
		}
	}
}

bool ChunkGenerator::hasGeneratedChunks()
{
	std::lock_guard<std::mutex> lock(generatedMutex);
	return (!generatedChunks.empty());
}

std::unique_ptr<Chunk> ChunkGenerator::fetchGeneratedChunk()
{
	std::lock_guard<std::mutex> lock(generatedMutex);
	if (generatedChunks.empty()) return nullptr;

	auto chunk = std::move(generatedChunks.front());
	generatedChunks.pop();
	return chunk;
}

std::unique_ptr<Chunk> ChunkGenerator::generateChunk(const ChunkCoord& coord) 
{
	auto chunk = std::make_unique<Chunk>(coord, world->getSeed());

	for (int x = 0; x < Chunk::SIZEX; x++)
	{
		for (int z = 0; z < Chunk::SIZEZ; z++)
		{
			int wx = coord.x * Chunk::SIZEX + x;
			int wz = coord.z * Chunk::SIZEZ + z;

			int height = heightProvider->getHeight(wx, wz);

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
	}

	std::lock_guard<std::mutex> lock(decoratorMutex);
	for (auto& decorator : decorators)
	{
		decorator->decorate(*chunk, coord);
	}

	return chunk;
}

void ChunkGenerator::markChunkDirty(const ChunkCoord& coord)
{
	if (chunkDirtyMap[coord]) return;

	chunkDirtyMap[coord] = true;
	dirtyQueue.push(coord);
}

bool ChunkGenerator::hasDirtyChunks()
{
	return (!dirtyQueue.empty());
}

ChunkCoord ChunkGenerator::popDirtyChunk()
{
	if (!hasDirtyChunks()) return { 0, 0 };

	ChunkCoord coord = dirtyQueue.front();
	dirtyQueue.pop();

	chunkDirtyMap[coord] = false;

	return coord;
}

void ChunkGenerator::processChunks(int chunksPerFrame)
{
	for (int i = 0; i < chunksPerFrame && !chunkQueue.empty(); i++)
	{
		ChunkCoord coord = chunkQueue.front();
		chunkQueue.pop();

		auto chunk = generateChunk(coord);
		world->addChunk(coord, std::move(chunk));

		markChunkDirty(coord);

		markChunkDirty({ coord.x + 1, coord.z });
		markChunkDirty({ coord.x - 1, coord.z });
		markChunkDirty({ coord.x, coord.z + 1 });
		markChunkDirty({ coord.x, coord.z - 1 });
	}
}