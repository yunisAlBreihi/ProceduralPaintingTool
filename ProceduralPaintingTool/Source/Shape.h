#pragma once
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Transform.h"

class Shape
{
private:
	Shader m_shader;

	GLuint m_VAO = -1;
	GLuint m_VBO = -1;
	GLuint m_EBO = -1;

	unsigned int m_vertexCount = 0;

	glm::mat4 m_position_matrix;
	glm::mat4 m_rotation_matrix;
	glm::mat4 m_scale_matrix;

	Transform m_transform;

public:
	Shape(const std::vector<float>& vertices, const std::vector<int>& indices, Transform transform, Shader shader);

	void update();
	void render(GLFWwindow* window);
	void setPosition(const glm::vec3& position);
	void setRotation(const glm::vec3& rotation);
	void setScale(const glm::vec3& scale);

	Transform& getTransform() { return m_transform; }

	void updateTransform();
};