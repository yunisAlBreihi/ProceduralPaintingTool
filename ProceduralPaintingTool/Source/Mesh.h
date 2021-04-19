#pragma once
#include <glad/glad.h>
#include "Shader.h"
#include "Transform.h"
#include "Rendering.h"
#include <vector>
#include "Vertex.h"


class Mesh
{
public:
	GLuint m_vao = -1;
	GLuint m_vbo = -1;
	int m_draw_count = 0;

	glm::mat4 m_position_matrix = glm::identity<glm::mat4>();
	glm::mat4 m_rotation_matrix = glm::identity<glm::mat4>();
	glm::mat4 m_scale_matrix = glm::identity<glm::mat4>();

	std::vector<Vertex*> m_areaVertices;
	Vertex* m_verts = nullptr;
	int m_vert_count = 0;

	Transform m_transform;
private:
	void mesh_load(const char* path);
	void update_bufferData();
	
public:
	Mesh() {}
	Mesh(const char* path, Transform transform);

	void setPosition(const glm::vec3& position);
	void setRotation(const float& angle, const glm::vec3& axis);
	void setVertexColor(int vertexIndex, const glm::vec4& color);
	void setVertexColor(int vertexIndex, const float* color);
	void setScale(const glm::vec3& scale);

	Transform& getTransform() { return m_transform; }
	const glm::mat4& getMatrix() { return m_position_matrix * m_rotation_matrix * m_scale_matrix; }

	const Vertex& getVertexAtPosition(glm::vec3& targetPosition, float thresholdRadius);
	std::vector<Vertex*>& getVertexAtPositionRadius(glm::vec3& targetPosition, float thresholdRadius);
	const Vertex& getVertexAtPositionFlat(glm::vec3& targetPosition, float thresholdRadius);

	const Vertex& getLowestVertexPositionFlat();
	const Vertex& getHighestVertexPositionFlat();
};