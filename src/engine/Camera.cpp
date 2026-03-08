#include "engine/Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) 
: front(glm::vec3(0.0f, 0.0f, -1.0f))
, movementSpeed(2.5f)
, mouseSensitivity(0.1f)
, position(position)
, up(up)
, yaw(yaw)
, pitch(pitch)
{
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(int key, float deltaTime)
{
	float cameraSpeed = static_cast<float>(movementSpeed * deltaTime);
	if (key == GLFW_KEY_W)
		position += cameraSpeed * front;
	if (key == GLFW_KEY_S)
		position -= cameraSpeed * front;
	if (key == GLFW_KEY_A)
		position -= glm::normalize(glm::cross(front, up)) * cameraSpeed;
	if (key == GLFW_KEY_D)
		position += glm::normalize(glm::cross(front, up)) * cameraSpeed;
	if (key == GLFW_KEY_SPACE)
		position += cameraSpeed * up;
	if (key == GLFW_KEY_LEFT_SHIFT)
		position -= cameraSpeed * up;
}

void Camera::processMouseMovement(float xoffset, float yoffset)
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 _front;
	_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	_front.y = sin(glm::radians(pitch));
	_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(_front);
}