#pragma once
#include <glad/glad.h>
#include "Shader.h"
#include "Transform.h"
#include "Rendering.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texcoord;
	glm::vec3 normal;
	glm::vec4 color;

	int index;
};

class Mesh
{
public:
	GLuint m_vao = -1;
	GLuint m_vbo = -1;
	int m_draw_count = 0;

	glm::mat4 m_position_matrix = glm::identity<glm::mat4>();
	glm::mat4 m_rotation_matrix = glm::identity<glm::mat4>();
	glm::mat4 m_scale_matrix = glm::identity<glm::mat4>();

	Vertex* m_verts = nullptr;
	int m_vert_count = 0;

	Transform m_transform;
private:
	void mesh_load(const char* path);
	void update_bufferData();
	
public:
	Mesh() {}
	Mesh(const char* path, Transform transform);
	//void update();

	void setPosition(const glm::vec3& position);
	void setRotation(const float& angle, const glm::vec3& axis);
	void setVertexColor(int vertexIndex, const glm::vec4& color);
	void setScale(const glm::vec3& scale);

	Transform& getTransform() { return m_transform; }
	const glm::mat4& getMatrix() { return m_position_matrix * m_rotation_matrix * m_scale_matrix; }

	const Vertex& getVertexAtPosition(glm::vec3& targetPosition, float thresholdRadius);
	const Vertex& getVertexAtFlatPosition(glm::vec3& targetPosition, float thresholdRadius);
};