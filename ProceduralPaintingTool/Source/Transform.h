#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Transform {
public:
	glm::vec3 position = glm::vec3(0.0f);
	glm::quat rotation = glm::quat();
	glm::vec3 scale = glm::vec3(1.0f);

public:
	Transform() {}
	Transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale) : position(position), rotation(rotation), scale(scale) {}
};