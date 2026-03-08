// 3dEngine.cpp : Defines the entry point for the application.
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "engine/Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "engine/Camera.h"
#include "engine/texture/Texture.h"
#include "engine/mesh/Mesh.h"
#include "engine/Engine.h"

int main()
{
	Engine engine;
	return engine.run();
}
