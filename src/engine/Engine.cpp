#include "engine/Engine.h"
#include <iostream>

double fpsTimer = 0.0;
int frameCount = 0;

Engine::Engine()
{
	init();
}

int Engine::init()
{
	std::cout << "working" << std::endl;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1600, 1200, "3d Engine", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 1600, 1200);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	camera = std::make_unique<Camera>(glm::vec3(0.0f, 100.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
	shader = std::make_unique<Shader>("shaders/vert.glsl", "shaders/frag.glsl");

	meshManager = std::make_unique<MeshManager>();
	textureManager = std::make_unique<TextureManager>();

	setupCallbacks();

	scene = std::make_unique<Scene>();

	shader->use();
	glm::mat4 model = glm::mat4(1.0f);

	shader->setMat4("model", model);

	Texture texture("textures/textureAtlas.jpg");

	return 0;
}

int Engine::run()
{
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		fpsTimer += deltaTime;
		frameCount++;

		if (fpsTimer >= 1.0)
		{
			int fps = frameCount;
			std::string title = "3D Engine - FPS: " + std::to_string(fps);
			glfwSetWindowTitle(window, title.c_str());

			frameCount = 0;
			fpsTimer = 0.0;
		}

		processInput();
		update(deltaTime);
		render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void Engine::update(float deltaTime)
{
	// Update game logic here
}

void Engine::render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->use();

	glm::mat4 view = camera->getViewMatrix();
	shader->setMat4("view", view);

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(90.0f), sizex / sizey, 0.1f, 1000.0f);
	shader->setMat4("projection", projection);

	scene->update(deltaTime);
	scene->render(*shader);
}

void Engine::processInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = static_cast<float>(2.5 * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->processKeyboard(GLFW_KEY_W, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->processKeyboard(GLFW_KEY_S, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->processKeyboard(GLFW_KEY_A, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->processKeyboard(GLFW_KEY_D, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera->processKeyboard(GLFW_KEY_SPACE, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera->processKeyboard(GLFW_KEY_LEFT_SHIFT, deltaTime);
}

void Engine::setupCallbacks()
{
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, Engine::framebuffer_size_callback);
	glfwSetCursorPosCallback(window, Engine::mouse_callback);
}

void Engine::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	Engine* engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
	engine->sizex = static_cast<float>(width);
	engine->sizey = static_cast<float>(height);
	glViewport(0, 0, width, height);
}

void Engine::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	Engine* engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));

	float xoffset = xpos - engine->lastX;
	float yoffset = engine->lastY - ypos;

	engine->lastX = xpos;
	engine->lastY = ypos;

	engine->camera->processMouseMovement(xoffset, yoffset);
}