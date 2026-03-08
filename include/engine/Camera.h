#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;

	float yaw;
	float pitch;
	float movementSpeed;
	float mouseSensitivity;

	Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);

	glm::mat4 getViewMatrix();
	void processKeyboard(int key, float deltaTime);
	void processMouseMovement(float xoffset, float yoffset);
};