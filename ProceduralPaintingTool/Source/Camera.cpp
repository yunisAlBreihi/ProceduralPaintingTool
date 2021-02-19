#include "Camera.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "globals.h"

Camera::Camera(const glm::vec3& position, GLFWwindow* window, ObjectManager& objectManager) : m_position(position), m_window(window), m_objectManager(objectManager)
{

}

void Camera::update(const float& deltaTime)
{


	glm::vec3 t_cam_rot = glm::vec3(0.0f);
	if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		t_cam_rot.y -= glm::radians(90.0f);
	}
	if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		t_cam_rot.y += glm::radians(90.0f);
	}
	if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS) {
		t_cam_rot.x -= glm::radians(90.0f);
	}
	if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		t_cam_rot.x += glm::radians(90.0f);
	}
	m_rotation += t_cam_rot * glm::radians(90.0f) * deltaTime;
	glm::quat t_cam_quat = glm::angleAxis(m_rotation.y, globals::UP) *
						   glm::angleAxis(m_rotation.x, globals::RIGHT) * 
						   glm::angleAxis(m_rotation.z, globals::FORWARD);

	m_right = t_cam_quat * globals::RIGHT;
	m_forward = t_cam_quat * globals::FORWARD;

	//Move cam
	glm::vec3 t_camera_move = glm::vec3(0.0f);
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
		t_camera_move -= m_right;
	}
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
		t_camera_move += m_right;
	}
	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
		t_camera_move += m_forward;
	}
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
		t_camera_move -= m_forward;
	}
	if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		t_camera_move.y += 1.0f;
	}
	if (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		t_camera_move.y -= 1.0f;
	}
	m_position += t_camera_move * m_moveSpeed * deltaTime;

	int t_winWidth, t_winHeight;
	glfwGetWindowSize(m_window, &t_winWidth, &t_winHeight);

	glm::mat4 t_projection = glm::perspective(glm::radians(90.0f), t_winWidth / (float)t_winHeight, m_nearClip, m_farClip);
	glm::mat4 t_view = glm::lookAt(m_position, m_position + m_forward, globals::UP);
	glm::mat4 t_view_matrix = t_projection * t_view;

	for (auto& t_shader : m_objectManager.getShaders()) {
		t_shader.setMatrix4(globals::VIEWPROJECTION, t_view_matrix);
		t_shader.use();
	}
}
