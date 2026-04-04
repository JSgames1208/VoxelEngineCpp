//
// Created by Jasper Soete on 4-4-2026.
//

#pragma once
#include "engine/Game.h"
#include <vector>
#include <memory>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <glm/gtc/matrix_transform.hpp>
#include "engine/Shader.h"
#include "engine/world/Chunk.h"
#include "engine/mesh/ChunkMesher.h"
#include "engine/renderer/chunk/ChunkRenderer.h"
#include "engine/mesh/ThreadedMeshCreator.h"
#include "engine/mesh/Mesh.h"
#include "engine/world/Level.h"
#include "engine/core/ChunkCoord.h"
#include "engine/gen/ChunkGenerator.h"
#include "engine/mesh/ThreadedChunkMesher.h"
#include <chrono>


struct ChunkMesh
{
    ChunkMesh(std::unique_ptr<Mesh> _mesh, const ChunkCoord& _coord)
            : mesh(std::move(_mesh)), coord(_coord)
    {
    }

    std::unique_ptr<Mesh> mesh;
    ChunkCoord coord;
};


class VoxelGame : public Game
{
public:
    VoxelGame();
    ~VoxelGame();

    void update(float deltaTime);
    void render(Shader& shader);
private:
    void markChunkDirty(const ChunkCoord& coord);
    void updateChunkMesh(const ChunkCoord& coord, std::unique_ptr<Mesh> mesh);
private:
    std::unique_ptr<ChunkMesher> mesher;
    std::vector<Chunk> chunks;
    std::unordered_map<ChunkCoord, std::unique_ptr<ChunkMesh>> chunkMeshes;
    std::unique_ptr<Level> world;
    std::unique_ptr<ChunkGenerator> generator;
    std::unique_ptr<ThreadedChunkMesher> threadedMesher;
    std::unique_ptr<ThreadedMeshCreator> meshCreator;
    std::queue<ChunkCoord> dirtyQueue;
    std::unique_ptr<TextureAtlas> atlas;
    ChunkRenderer chunkRenderer;

    std::queue<std::pair<ChunkCoord, std::unique_ptr<Mesh>>> gpuUploadQueue;

    int totalChunksToGenerate = 0;
    int generatedCount = 0;
    bool timingStarted = false;

    bool meshTimingStarted = false;

    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point meshStartTime;
};
