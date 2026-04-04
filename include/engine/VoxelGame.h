//
// Created by Jasper Soete on 4-4-2026.
//

#pragma once
#include "engine/Game.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
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
#include "engine/main/GameConfig.h"
#include "engine/texture/Texture.h"
#include "engine/Camera.h"
#include "engine/platform/Window.h"
#include "engine/platform/KeyboardHandler.h"
#include "engine/platform/MouseHandler.h"
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
    VoxelGame(const GameConfig& gameConfig);
    ~VoxelGame();

    int run();
    int initWindow();
    void update(float deltaTime);
    void render();

    Window* getWindow();
    float getDeltaTime();
    Camera* getCamera();

    float lastX = 400.0f;
    float lastY = 300.0f;
private:
    void markChunkDirty(const ChunkCoord& coord);
    void updateChunkMesh(const ChunkCoord& coord, std::unique_ptr<Mesh> mesh);

    void processInput();
    void setupCallbacks();
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

    const GameConfig& gameConfig;

    float sizex = 2400.0f;
    float sizey = 1800.0f;

    std::unique_ptr<Window> window;
    std::unique_ptr<KeyboardHandler> keyboardHandler;
    std::unique_ptr<MouseHandler> mouseHandler;

    std::unique_ptr<Camera> camera;
    std::unique_ptr<Shader> shader;
    std::unique_ptr<Texture> texture;
    std::unique_ptr<Mesh> mesh;

    std::queue<std::pair<ChunkCoord, std::unique_ptr<Mesh>>> gpuUploadQueue;

    int totalChunksToGenerate = 0;
    int generatedCount = 0;
    bool timingStarted = false;

    bool meshTimingStarted = false;

    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point meshStartTime;

    float lastFrame = 0.0f;
    float deltaTime = 0.0f;

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
};
