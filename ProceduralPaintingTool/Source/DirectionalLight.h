#pragma once
#include "glm/ext/matrix_transform.hpp"
#include "Shader.h"


class DirectionalLight
{
private:

public:
	void update(Shader* shader, const glm::vec3& direction, const glm::vec3& cameraPosition);
};