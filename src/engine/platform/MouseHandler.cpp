//
// Created by Jasper Soete on 4-4-2026.
//

#include "engine/VoxelGame.h"
#include "engine/platform/MouseHandler.h"

MouseHandler::MouseHandler(VoxelGame *game)
    : game(game)
{}

void MouseHandler::setup()
{
    if (!game)
    {
        throw std::exception("Game not initialized!");
    }
}

void MouseHandler::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    VoxelGame* game = reinterpret_cast<VoxelGame*>(glfwGetWindowUserPointer(window));

    float xoffset = xpos - game->lastX;
    float yoffset = game->lastY - ypos;

    game->lastX = xpos;
    game->lastY = ypos;

    game->getCamera()->processMouseMovement(xoffset, yoffset);
}