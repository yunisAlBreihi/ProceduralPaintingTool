#include "Debug.h"
#include <glad/glad.h>


DebugLine::DebugLine(const glm::vec3& position, const glm::vec3& direction, float length, float lifeTime) :
	m_position(position), m_direction(direction), m_length(length), m_maxLifeTime(lifeTime) {
	m_verts = new Vertex[m_vert_count];

	//Start vertex
	m_verts[0].position = m_position;

	//End vertex
	m_verts[1].position = m_position + (m_direction * m_length);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vert_count, m_verts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), 0);
}

void DebugLine::update(float deltaTime) {
	if (m_LifeTime < m_maxLifeTime) {
		m_LifeTime += deltaTime;
	}
}

bool DebugLine::active() {
	if (m_LifeTime < m_maxLifeTime) {
		return true;
	}
	return false;
}
