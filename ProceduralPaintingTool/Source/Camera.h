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

	float m_yaw = 0.0f;
	float m_pitch = 0.0f;
	float m_fov = 60.0f;
	float m_aspect = 1.0f;
	float m_nearClip = 0.1f;
	float m_farClip = 100.0f;

	float m_moveSpeed = 3.0f;

	float ortho_height = 5.0f;

public:
	Camera(const glm::vec3& position, GLFWwindow* window, ObjectManager& objectManager);

	void update(const float& deltaTime);

	const glm::vec3& getPosition() { return m_position; }
};

