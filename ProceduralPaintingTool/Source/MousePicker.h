#pragma once
#include "glm/glm.hpp"
#include "Camera.h"

class MousePicker 
{
private:
	glm::vec3 m_currentRay{};

	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;
	Camera* m_camera;

private:
	glm::vec3 calculateMouseRay();
	glm::vec4 toEyeCoords(glm::vec4 clipCoords);
	glm::vec2 getNormalizedDeviceCoords(glm::vec2 mousePosition);
	glm::vec3 toWorldCoords(glm::vec4 eyeCoords);

public:
	MousePicker(Camera* cam);

	void update();
	void printRay();

	glm::vec3 getCurrentRay() { return m_currentRay; }
};