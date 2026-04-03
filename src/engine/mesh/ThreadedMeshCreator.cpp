//
// Created by Jasper Soete on 3-4-2026.
//

#include "engine/mesh/ThreadedMeshCreator.h"
#include "engine/mesh/ThreadedChunkMesher.h"
#include <vector>
#include <memory>

ThreadedMeshCreator::ThreadedMeshCreator(ThreadedChunkMesher* mesher, int threadCount)
    : running(false)
    , mesher(mesher)
    , threadCount(threadCount)
{

}

ThreadedMeshCreator::~ThreadedMeshCreator() = default;

void ThreadedMeshCreator::start()
{
    threadBuffers.resize(threadCount);
    running = true;
    for (int i = 0; i < threadCount; ++i)
        workers.emplace_back(&ThreadedMeshCreator::workerLoop, this, i);
}

void ThreadedMeshCreator::stop()
{
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        running = false;
    }
    cv.notify_all();
    for (auto& t : workers) if (t.joinable()) t.join();
}

void ThreadedMeshCreator::queueQuads(const ChunkCoord &coord, std::unique_ptr<std::vector<BakedQuad>> quads)
{
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        creationQueue.push({coord, std::move(quads)});
    }
    cv.notify_one();
}

bool ThreadedMeshCreator::hasFinishedMeshes()
{
    std::lock_guard<std::mutex> lock(finishedMutex);
    return !finishedQueue.empty();
}

std::pair<ChunkCoord, std::unique_ptr<MeshData>> ThreadedMeshCreator::fetchFinishedMesh() {
    std::lock_guard<std::mutex> lock(finishedMutex);
    if (finishedQueue.empty()) return {ChunkCoord(0, 0), nullptr};
    auto item = std::move(finishedQueue.front());
    finishedQueue.pop();
    return item;
}

void ThreadedMeshCreator::workerLoop(int threadIndex)
{
    while (true)
    {
        Item item;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            cv.wait(lock, [&]{ return !creationQueue.empty() || !running; });
            if (!running && creationQueue.empty()) return;
            item = std::move(creationQueue.front());
            creationQueue.pop();
        }

        std::unique_ptr<MeshData> meshData = mesher->createMeshFromQuads(*item.quads);

        threadBuffers[threadIndex].emplace_back(item.coord, std::move(meshData));
    }
}

void ThreadedMeshCreator::mergeThreadLocalFinished()
{
    std::lock_guard<std::mutex> lock(finishedMutex);

    for (auto& buffer : threadBuffers)
    {
        for (auto& item : buffer)
        {
            finishedQueue.emplace(std::move(item));
        }
        buffer.clear();
    }
}