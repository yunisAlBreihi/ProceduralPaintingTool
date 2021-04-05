#pragma once
#include <glm/glm.hpp>

namespace globals
{
	static const glm::vec3 RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);
	static const glm::vec3 LEFT = glm::vec3(-1.0f, 0.0f, 0.0f);
	static const glm::vec3 FORWARD = glm::vec3(0.0f, 0.0f, -1.0f);
	static const glm::vec3 BACK = glm::vec3(0.0f, 0.0f, 1.0f);
	static const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
	static const glm::vec3 ZERO = glm::vec3(0.0f);
	static const glm::vec3 ONE = glm::vec3(1.0f);
	static const float MAX_CAM_PITCH = 1.57f;
	static const float MIN_CAM_PITCH = -1.57f;

	static const char* VIEWPROJECTION = "u_ViewProjection";

	extern bool g_cam_control;
	extern bool g_LMB_hold;
	extern glm::vec2 g_mouse_position;
	extern glm::vec2 g_mouse_delta;
	extern glm::vec2 g_window_size;

	void printVec3(const glm::vec3& vec3);
};