#pragma once
#include <glm/glm.hpp>

struct Vertex {
	glm::vec3 position;
	glm::vec2 texcoord;
	glm::vec3 normal;
	glm::vec4 color;

	int index;
};