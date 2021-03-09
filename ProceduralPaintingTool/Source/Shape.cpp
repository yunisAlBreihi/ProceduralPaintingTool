#include "Shape.h"

Shape::Shape(const std::vector<float>& vertices, const std::vector<int>& indices, Transform transform, Shader shader) : m_shader(shader),m_transform(transform), m_vertexCount(indices.size())
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	//Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	//Bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

	//Position Attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//setPosition(m_transform.position);
	//setRotation(m_transform.rotation);
	//setScale(m_transform.scale);
}

void Shape::update()
{
	setPosition(m_transform.position);
	setRotation(m_transform.rotation);
	setScale(m_transform.scale);
}

void Shape::render(GLFWwindow* window)
{
	m_shader.use();
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_vertexCount, GL_UNSIGNED_INT, 0);
}

void Shape::setPosition(const glm::vec3& position)
{
	m_transform.position = position;
	m_position_matrix = glm::translate(glm::identity<glm::mat4>(), m_transform.position);
	updateTransform();
}

void Shape::setRotation(const glm::vec3& rotation)
{
	m_transform.rotation = rotation;
	m_rotation_matrix = glm::rotate(glm::identity<glm::mat4>(), glm::radians(m_transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_rotation_matrix = glm::rotate(m_rotation_matrix, glm::radians(m_transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	m_rotation_matrix = glm::rotate(m_rotation_matrix, glm::radians(m_transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	updateTransform();
}

void Shape::setScale(const glm::vec3& scale)
{
	m_transform.scale = scale;
	m_scale_matrix = glm::scale(glm::identity<glm::mat4>(), m_transform.scale);
	updateTransform();
}

void Shape::updateTransform()
{
	glm::mat4 t_model_matrix = m_position_matrix * m_rotation_matrix * m_scale_matrix;
	m_shader.set_attribute("u_Model", t_model_matrix);
}
