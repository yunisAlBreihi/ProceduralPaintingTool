#pragma once
#include "globals.h"
#include <vector>
#include "Shader.h"
#include "ObjectManager.h"

struct GLFWwindow;

class Camera
{
private:
	ObjectManager& m_objectManager;
	GLFWwindow* m_window;

	glm::vec3 m_position = glm::vec3(0.0f);
	glm::vec3 m_rotation = glm::vec3(0.0f);
	glm::vec3 m_right = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 m_forward = glm::vec3(0.0f, 0.0f, -1.0f);

	float m_nearClip = 0.25f;
	float m_farClip = 10.0f;
	float m_moveSpeed = 3.0f;

public:
	Camera(const glm::vec3& position, GLFWwindow* window, ObjectManager& objectManager);

	void update(const float& deltaTime);
};

