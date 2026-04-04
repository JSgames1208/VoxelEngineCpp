//
// Created by Jasper Soete on 4-4-2026.
//

#pragma once
#include "GLFW/glfw3.h"

class VoxelGame;

class MouseHandler
{
public:
    MouseHandler(VoxelGame* game);
    void setup();
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
private:
    VoxelGame* game;
};
