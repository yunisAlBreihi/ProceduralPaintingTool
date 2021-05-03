#include "MousePicker.h"

MousePicker::MousePicker(Camera* cam) : m_camera(cam) {
	m_viewMatrix = m_camera->viewMatrix();
	m_projectionMatrix = m_camera->projectionMatrix();
}

void MousePicker::update() {
	m_viewMatrix = m_camera->viewMatrix();
	m_currentRay = calculateMouseRay();
}

void MousePicker::printRay() {
	printf("CurrentRay X: %0.1f, Y: %0.1f, Z: %0.1f\n", m_currentRay.x, m_currentRay.y, m_currentRay.z);
}

glm::vec3 MousePicker::calculateMouseRay() {
	glm::vec2 normalizedCoords = getNormalizedDeviceCoords(globals::g_mouse_position);
	glm::vec4 clipCoords{normalizedCoords.x, normalizedCoords.y, -1.0f, 1.0f};
	glm::vec4 eyeCoords = toEyeCoords(clipCoords);
	glm::vec3 worldRay = toWorldCoords(eyeCoords);
	return worldRay;
}

glm::vec4 MousePicker::toEyeCoords(glm::vec4 clipCoords) {
	glm::mat4 invertedProjection = glm::inverse(m_projectionMatrix);
	glm::vec4 eyeCoords = invertedProjection * clipCoords;
	return glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
}

glm::vec3 MousePicker::toWorldCoords(glm::vec4 eyeCoords) {
	glm::mat4 invertedView = glm::inverse(m_viewMatrix);
	glm::vec4 rayWorld = invertedView * eyeCoords;
	glm::vec3 mouseRay{rayWorld.x, rayWorld.y, rayWorld.z};
	return glm::normalize(mouseRay);
}

glm::vec2 MousePicker::getNormalizedDeviceCoords(glm::vec2 mousePosition) {
	float x = (2.0f * mousePosition.x) / globals::g_window_size.x - 1;
	float y = (2.0f * mousePosition.y) / globals::g_window_size.y - 1;
	return glm::vec2(x, -y);
}
