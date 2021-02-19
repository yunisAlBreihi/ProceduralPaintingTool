#pragma once
#include <glad/glad.h>
#include "Shader.h"
#include "Transform.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texcoord;
	glm::vec3 normal;
};

class Mesh 
{
private:
	GLuint m_vao = -1;
	GLuint m_vbo = -1;
	int m_draw_count = 0;
	Shader m_shader;

	glm::mat4 m_position_matrix;
	glm::mat4 m_rotation_matrix;
	glm::mat4 m_scale_matrix;

	Transform m_transform;

private:
	void mesh_load(const char* path);
	void updateTransform();

public:
	Mesh(const char* path, Transform transform, Shader shader);
	void update();
	void render();

	void setPosition(const glm::vec3& position);
	void setRotation(const glm::vec3& rotation);
	void setScale(const glm::vec3& scale);

	Transform& getTransform() { return m_transform; }
};