#pragma once
#include "globals.h"
#include <vector>
#include "Shader.h"

struct GLFWwindow;

class Camera
{
private:
	std::vector<Shader>& m_shaders;
	GLFWwindow* m_window;

	glm::vec3 m_position = glm::vec3(0.0f);
	glm::vec3 m_right = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 m_forward = glm::vec3(0.0f, 0.0f, -1.0f);

	float m_nearClip = 0.25f;
	float m_farClip = 10.0f;
	float m_moveSpeed = 3.0f;

public:
	Camera(const glm::vec3& position, GLFWwindow* window, std::vector<Shader>& shaders);

	void update(const float& deltaTime);
};

