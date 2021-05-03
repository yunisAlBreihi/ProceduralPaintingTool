#pragma once
#include "glm/ext/matrix_transform.hpp"
#include "Debug.h"

class Mesh;
class Shader;

struct RenderData {
	glm::mat4 m_view_projection = glm::identity<glm::mat4>();
	glm::mat4 m_model = glm::identity<glm::mat4>();

	Shader* m_shader = nullptr;

	//Lights
	glm::vec3 m_directional_light;
	glm::vec3 m_eye_position;
};

void mesh_draw(const Mesh& mesh, const RenderData& data);
void debugLine_draw(const DebugLine& debugLine, const RenderData& data);