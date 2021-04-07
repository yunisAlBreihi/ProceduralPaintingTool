#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "Vertex.h"

class DebugLine
{
private:


	Vertex* m_verts = nullptr;
	const int m_vert_count = 2;

	glm::vec3 m_position{ 0.0f };
	glm::vec3 m_direction{ 0.0f };
	float m_length = 0.0f;
	float m_maxLifeTime = 0.0f;

	float m_LifeTime = 0.0f;

public:
	GLuint m_vao = -1;
	GLuint m_vbo = -1;
	const int m_draw_count = m_vert_count;

public:
	DebugLine(const glm::vec3& position, const glm::vec3& direction, float length, float lifeTime);
	void update(float deltaTime);

	bool active();
};