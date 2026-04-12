//
// Created by Jasper Soete on 4-4-2026.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/VoxelGame.h"
#include "engine/world/Level.h"
#include "engine/gen/decorators/TreeDecorator.h"
#include "engine/gen/decorators/RockClumpDecorator.h"
#include "Profiler.h"

double fpsTimer = 0.0;
int frameCount = 0;

VoxelGame::VoxelGame(const GameConfig& gameConfig)
    : gameConfig(gameConfig)
{
    world = std::make_unique<Level>();

    // 2. Create the atlas
    atlas = std::make_unique<TextureAtlas>();
    if (!atlas->loadFromJSON()) {
        std::cerr << "Failed to load textures!\n";
        return;
    }

    keyboardHandler = std::make_unique<KeyboardHandler>(this);
    keyboardHandler->setup();

    mouseHandler = std::make_unique<MouseHandler>(this);
    mouseHandler->setup();

    // 3. Create the mesher with the atlas pointer
    mesher = std::make_unique<ChunkMesher>(atlas.get());

    // 4. Threaded mesher gets a pointer to mesher (safe)
    threadedMesher = std::make_unique<ThreadedChunkMesher>(world.get(), mesher.get());
    meshCreator = std::make_unique<ThreadedMeshCreator>(threadedMesher.get());

    // 5. Chunk generator
    generator = std::make_unique<ChunkGenerator>(world.get());
    generator->addDecorator(std::make_unique<TreeDecorator>());
    generator->addDecorator(std::make_unique<RockClumpDecorator>());

    std::cout << "loaded generator and mesher" << std::endl;

    // 6. Queue chunks
    int renderDistance = 30;
    totalChunksToGenerate = (2 * renderDistance + 1) * (2 * renderDistance + 1);
    for (int r = 0; r <= renderDistance; r++)
        for (int x = -r; x <= r; x++)
            for (int z = -r; z <= r; z++)
                if (abs(x) == r || abs(z) == r)
                    generator->queueChunk({ x, z });

    // 7. Start timing and generation
    startTime = std::chrono::high_resolution_clock::now();
    timingStarted = true;
}

int VoxelGame::initWindow()
{
    std::cout << "~~3D Voxel Engine~~" << std::endl;
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }

    const DisplayData& displayData = gameConfig.getDisplayData();
    window = std::make_unique<Window>(gameConfig.getDisplayData(), "~~3D Voxel Engine~~");
    if (!window || !window->getHandle())
    {
        std::cout << "Window creation failed!" << std::endl;
        return -1;
    }

    sizex = displayData.width;
    sizey = displayData.height;

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        return -1;
    }

    camera = std::make_unique<Camera>(this, glm::vec3(0.0f, 150.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
    shader = std::make_unique<Shader>("shaders/vert.glsl", "shaders/frag.glsl");

    setupCallbacks();

    shader->use();
    glm::mat4 model = glm::mat4(1.0f);

    shader->setMat4("model", model);

    texture = std::make_unique<Texture>("textures/texture_atlas.png");

    return 0;
}

int VoxelGame::run()
{
    if (initWindow() != 0)
    {
        std::cout << "Failed to create window!" << std::endl;
        return -1;
    }

    meshCreator->start();
    threadedMesher->start();
    generator->start();

    while (!glfwWindowShouldClose(window->getHandle()))
    {
        profiler::Profiler::get().beginFrame();
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        fpsTimer += deltaTime;
        frameCount++;

        if (fpsTimer >= 1.0)
        {
            int fps = frameCount;
            std::string title = "3D Engine - FPS: " + std::to_string(fps);
            glfwSetWindowTitle(window->getHandle(), title.c_str());

            frameCount = 0;
            fpsTimer = 0.0;
        }

        keyboardHandler->process();
        update(deltaTime);
        render();

        glfwSwapBuffers(window->getHandle());
        glfwPollEvents();
        profiler::Profiler::get().endFrame();
    }

    glfwTerminate();
    return 0;
}

VoxelGame::~VoxelGame()
{
    generator->stop();
    threadedMesher->stop();
    meshCreator->stop();
}

Window* VoxelGame::getWindow()
{
    return this->window.get();
}

float VoxelGame::getDeltaTime()
{
    return deltaTime;
}

Camera* VoxelGame::getCamera()
{
    return camera.get();
}

void VoxelGame::update(float deltaTime)
{
    PROFILE_SCOPE("update");
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

    int meshesPerFrame = 1000;
    for (int i = 0; i < meshesPerFrame && threadedMesher->hasFinishedMeshes(); ++i)
    {
        auto [coord, quads] = threadedMesher->fetchFinishedMesh();
        if (!quads) continue;

        meshCreator->queueQuads(coord, std::move(quads));
    }

    meshCreator->mergeThreadLocalFinished();
    int meshCreationsPerFrame = 1000;
    for (int i = 0; i < meshCreationsPerFrame && meshCreator->hasFinishedMeshes(); ++i)
    {
        auto [coord, meshData] = meshCreator->fetchFinishedMesh();
        if (!meshData) continue;

        auto mesh = std::make_unique<Mesh>();

        mesh->setData(std::move(meshData));

        gpuUploadQueue.push({coord, std::move(mesh)});

        Chunk* chunk = world->getChunkPtr(coord);
        if (chunk)
        {
            chunk->isDirty = false;
            chunk->isQueued = false;
        }
    }


    int maxUploadsPerFrame = 1000;
    for (int i = 0; i < maxUploadsPerFrame && !gpuUploadQueue.empty(); ++i)
    {
        auto [coord, mesh] = std::move(gpuUploadQueue.front());
        gpuUploadQueue.pop();

        chunkMeshes[coord] = std::make_unique<ChunkMesh>(std::move(mesh), coord);
    }
}

void VoxelGame::markChunkDirty(const ChunkCoord& coord)
{
    Chunk* chunk = world->getChunkPtr(coord);
    if (!chunk) return;

    chunk->isDirty = true;
    chunk->isQueued = true;

    dirtyQueue.push(coord);
}

void VoxelGame::updateChunkMesh(const ChunkCoord& coord, std::unique_ptr<Mesh> mesh)
{
    chunkMeshes[coord] = std::make_unique<ChunkMesh>(std::move(mesh), coord);
}

void VoxelGame::render()
{
    PROFILE_SCOPE("render");
    glClearColor(130.0f / 256.0f, 200.0f / 256.0f, 229.0f / 256.0f, 1.0f);
    //glClearColor(255.0f / 256.0f, 45.0f / 256.0f, 209.0f / 256.0f, 1.0f); // CURSED PURPLE (:<
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->use();

    glm::mat4 view = camera->getViewMatrix();
    shader->setMat4("view", view);

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(90.0f), sizex / sizey, 0.1f, 1000.0f);
    shader->setMat4("projection", projection);

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
        shader->setMat4("model", model);

        chunkMesh->mesh->draw();
    }
}

void VoxelGame::setupCallbacks()
{
    glfwSetWindowUserPointer(window->getHandle(), this);
    glfwSetFramebufferSizeCallback(window->getHandle(), VoxelGame::framebuffer_size_callback);
    glfwSetCursorPosCallback(window->getHandle(), mouseHandler->mouseCallback);
}

void VoxelGame::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    VoxelGame* game = reinterpret_cast<VoxelGame*>(glfwGetWindowUserPointer(window));
    game->sizex = static_cast<float>(width);
    game->sizey = static_cast<float>(height);
    glViewport(0, 0, width, height);
}