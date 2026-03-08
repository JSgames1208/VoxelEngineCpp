#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/Camera.h"
#include "engine/texture/Texture.h"
#include "engine/Shader.h"
#include "engine/mesh/Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <memory>
#include "engine/scene/Scene.h"
#include "engine/texture/TextureManager.h"
#include "engine/mesh/MeshManager.h"

class Engine
{
public:
	Engine();
	int run();
	int init();
private:
	void update(float deltaTime);
	void render();
	void processInput();
	void setupCallbacks();

	float sizex = 800.0f;
	float sizey = 600.0f;

	float lastX = 400.0f;
	float lastY = 300.0f;

	GLFWwindow* window;

	std::unique_ptr<Camera> camera;
	std::unique_ptr<Shader> shader;
	std::unique_ptr<Texture> texture;
	std::unique_ptr<Mesh> mesh;

	std::unique_ptr<Scene> scene;
	std::unique_ptr<TextureManager> textureManager;
	std::unique_ptr<MeshManager> meshManager;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
};