#include "Camera.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "globals.h"

Camera::Camera(const glm::vec3& position, GLFWwindow* window) : m_position(position), m_window(window)
{

}

void Camera::update(const float& deltaTime)
{
	if (globals::g_cam_control == false) {
		return;
	}

	m_yaw -= globals::g_mouse_delta.x * m_rotateSpeed;
	m_pitch -= globals::g_mouse_delta.y * m_rotateSpeed;

	glm::vec3 t_move = glm::vec3(0.0f);
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
		t_move += right();
	}
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
		t_move -= right();
	}
	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
		t_move += forward();
	}
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
		t_move -= forward();
	}
	if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		t_move += globals::UP;
	}
	if (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		t_move -= globals::UP;
	}
	m_position += t_move * m_moveSpeed * deltaTime;
}

glm::mat4 Camera::matrix()
{
	glm::mat4 t_view_matrix = glm::lookAt(m_position, m_position + forward(), globals::UP);
	glm::mat4 t_proj_matrix;

	if (m_perspective == true) {
		t_proj_matrix = glm::perspective(glm::radians(m_fov), m_aspect, m_nearClip, m_farClip);
	}
	else {
		float t_half_height = m_ortho_height * 0.5f;
		t_proj_matrix = glm::ortho(-m_ortho_height * m_aspect, t_half_height * m_aspect, -t_half_height, t_half_height, m_nearClip, m_farClip);
	}
	return t_proj_matrix * t_view_matrix;
}

glm::quat Camera::quat()
{
	return glm::angleAxis(m_yaw, globals::UP) * glm::angleAxis(m_pitch, globals::RIGHT);
}

glm::vec3 Camera::forward()
{
	return quat() * globals::FORWARD;
}

glm::vec3 Camera::right()
{
	return quat() * globals::RIGHT;
}