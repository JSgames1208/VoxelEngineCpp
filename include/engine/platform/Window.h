//
// Created by Jasper Soete on 4-4-2026.
//

#pragma once
#include <iostream>
#include "GLFW/glfw3.h"
#include "engine/platform/DisplayData.h"

class Window
{
public:
    Window(const DisplayData& displayData, const std::string& title);
    GLFWwindow* createWindow(const std::string& title, int width, int height);
    void setWindowHints();
    GLFWwindow* getHandle();
    bool shouldClose();
    void close();
private:
    GLFWwindow* handle;
};
