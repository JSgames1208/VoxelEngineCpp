//
// Created by Jasper Soete on 4-4-2026.
//

#include "engine/platform/KeyboardHandler.h"
#include "engine/VoxelGame.h"

KeyboardHandler::KeyboardHandler(VoxelGame *game)
    : game(game)
{}

void KeyboardHandler::setup()
{
    if (!game)
    {
        throw std::exception("Game not initialized!");
    }
}

void KeyboardHandler::process()
{
    Window* window = this->game->getWindow();
    if (!window)
    {
        throw std::exception("Window is nullptr!");
    }

    if (glfwGetKey(window->getHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window->getHandle(), true);

    float deltaTime = game->getDeltaTime();
    Camera* camera = game->getCamera();
    if (!camera)
    {
        throw std::exception("Camera is nullptr!");
    }

    if (glfwGetKey(window->getHandle(), GLFW_KEY_W) == GLFW_PRESS)
        camera->processKeyboard(GLFW_KEY_W, deltaTime);
    if (glfwGetKey(window->getHandle(), GLFW_KEY_S) == GLFW_PRESS)
        camera->processKeyboard(GLFW_KEY_S, deltaTime);
    if (glfwGetKey(window->getHandle(), GLFW_KEY_A) == GLFW_PRESS)
        camera->processKeyboard(GLFW_KEY_A, deltaTime);
    if (glfwGetKey(window->getHandle(), GLFW_KEY_D) == GLFW_PRESS)
        camera->processKeyboard(GLFW_KEY_D, deltaTime);
    if (glfwGetKey(window->getHandle(), GLFW_KEY_SPACE) == GLFW_PRESS)
        camera->processKeyboard(GLFW_KEY_SPACE, deltaTime);
    if (glfwGetKey(window->getHandle(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera->processKeyboard(GLFW_KEY_LEFT_SHIFT, deltaTime);
}