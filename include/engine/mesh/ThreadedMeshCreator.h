//
// Created by Jasper Soete on 3-4-2026.
//

#pragma once
#include "engine/renderer/chunk/ChunkRenderer.h"
#include "engine/renderer/block/BakedQuad.h"
#include "engine/mesh/MeshData.h"
#include "engine/core/ChunkCoord.h"
#include "engine/mesh/Mesh.h"
#include <mutex>
#include <memory>
#include <queue>

class ThreadedChunkMesher;

class ThreadedMeshCreator
{
public:
    ThreadedMeshCreator(ThreadedChunkMesher* renderer, int threadCount = 1);
    ~ThreadedMeshCreator();

    void queueQuads(const ChunkCoord& coord, std::unique_ptr<std::vector<BakedQuad>> quads);

    bool hasFinishedMeshes();

    void start();
    void stop();

    std::pair<ChunkCoord, std::unique_ptr<MeshData>> fetchFinishedMesh();
    void mergeThreadLocalFinished();

    std::vector<std::vector<std::pair<ChunkCoord, std::unique_ptr<MeshData>>>> threadBuffers;
private:
    struct Item
    {
        ChunkCoord coord;
        std::unique_ptr<std::vector<BakedQuad>> quads;
    };

    int threadCount;

    std::queue<Item> creationQueue;
    std::queue<std::pair<ChunkCoord, std::unique_ptr<MeshData>>> finishedQueue;

    std::mutex queueMutex;
    std::mutex finishedMutex;
    std::condition_variable cv;
    bool running;
    std::vector<std::thread> workers;

    ThreadedChunkMesher* mesher;

    void workerLoop(int threadIndex);
};


