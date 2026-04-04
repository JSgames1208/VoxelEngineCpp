//
// Created by Jasper Soete on 4-4-2026.
//

#include "engine/platform/Window.h"

Window::Window(const DisplayData& displayData, const std::string& title)
{
    int width = displayData.width;
    int height = displayData.height;
    this->setWindowHints();
    handle = createWindow(title, width, height);
    glfwMakeContextCurrent(handle);
    glfwSwapInterval(0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

GLFWwindow* Window::getHandle()
{
    return handle;
}

GLFWwindow* Window::createWindow(const std::string& title, int width, int height)
{
    GLFWwindow* windowHandle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!windowHandle)
    {
        throw std::exception("Failed to create GLFW window");
    }
    return windowHandle;
}

void Window::setWindowHints()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

bool Window::shouldClose()
{
    return glfwWindowShouldClose(handle);
}

void Window::close()
{
    glfwDestroyWindow(handle);
    glfwTerminate();
}
