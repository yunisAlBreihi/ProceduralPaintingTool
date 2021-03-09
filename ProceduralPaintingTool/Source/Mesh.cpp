#include "Mesh.h"
#include "wavefront.h"
#include "globals.h"

Mesh::Mesh(const char* path, Transform transform) : m_transform()
{
	mesh_load(path);
	setPosition(transform.position);
	setRotation(0.0f, globals::UP);
	setScale(transform.scale);
}

void Mesh::setPosition(const glm::vec3& position)
{
	m_transform.position = position;
	m_position_matrix = translate(glm::identity<glm::mat4>(), position);
}

void Mesh::setRotation(const float& angle, const glm::vec3& axis)
{
	m_transform.rotation.x = axis.x * glm::sin(angle * 0.5f);
	m_transform.rotation.y = axis.y * glm::sin(angle * 0.5f);
	m_transform.rotation.z = axis.z * glm::sin(angle * 0.5f);
	m_transform.rotation.w = glm::cos(angle * 0.5f);

	m_rotation_matrix = glm::rotate(glm::mat4(1.0f), angle, axis);
}

void Mesh::setScale(const glm::vec3& scale)
{
	m_transform.scale = scale;
	m_scale_matrix = glm::scale(glm::mat4(1.0f), scale);
}

//void Mesh::update()
//{
//	updateTransform();
//}

//glm::mat4 Mesh::setPosition(const glm::vec3& position)
//{
//	m_transform.position = position;
//	m_position_matrix = glm::translate(glm::identity<glm::mat4>(), m_transform.position);
//	return updateTransform();
//}
//
//glm::mat4 Mesh::setRotation(const glm::vec3& rotation)
//{
//	m_transform.rotation = rotation;
//	m_rotation_matrix = glm::rotate(glm::identity<glm::mat4>(), glm::radians(m_transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
//	m_rotation_matrix = glm::rotate(m_rotation_matrix, glm::radians(m_transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
//	m_rotation_matrix = glm::rotate(m_rotation_matrix, glm::radians(m_transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
//	return updateTransform();
//}
//
//glm::mat4 Mesh::setScale(const glm::vec3& scale)
//{
//	m_transform.scale = scale;
//	m_scale_matrix = glm::scale(glm::identity<glm::mat4>(), m_transform.scale);
//	return updateTransform();
//}
//
//glm::mat4 Mesh::updateTransform()
//{
//	return m_position_matrix * m_rotation_matrix * m_scale_matrix;
//}

void Mesh::mesh_load(const char* path)
{
	Wavefront_File* obj = wavefront_load(path);
	if (obj == nullptr) {
		printf("Failed to load mesh: '%s', File doesn't exist\n", path);
		return;
	}

	Vertex* verts = new Vertex[obj->num_triangles * 3];
	int num_verts = 0;

	for (int tri_idx = 0; tri_idx < obj->num_triangles; ++tri_idx) {
		Wavefront_Triangle triangle = obj->triangles[tri_idx];
		for (int vert_idx = 0; vert_idx < 3; ++vert_idx) {
			Wavefront_Vertex& vertex = triangle.vertices[vert_idx];
			Vertex new_vertex;
			new_vertex.position = obj->positions[vertex.idx_position];
			new_vertex.normal = obj->normals[vertex.idx_normal];
			new_vertex.texcoord = obj->texcoords[vertex.idx_texcoord];

			verts[num_verts++] = new_vertex;
		}
	}
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * num_verts, verts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (void*)(sizeof(glm::vec3)));

	m_draw_count = num_verts;
}