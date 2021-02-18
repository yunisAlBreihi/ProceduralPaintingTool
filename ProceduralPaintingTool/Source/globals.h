#pragma once
#include <glm/glm.hpp>

namespace globals
{
	static const glm::vec3 RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);
	static const glm::vec3 FORWARD = glm::vec3(0.0f, 0.0f, -1.0f);
	static const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
	static const glm::vec3 ZERO = glm::vec3(0.0f);

	static const char* VIEWPROJECTION = "u_ViewProjection";
};

