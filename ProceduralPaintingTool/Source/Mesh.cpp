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
	m_position_matrix = translate(glm::identity<glm::mat4>(), m_transform.position);
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
	m_scale_matrix = glm::scale(glm::mat4(1.0f), m_transform.scale);
}

void Mesh::mesh_load(const char* path)
{
	Wavefront_File* obj = wavefront_load(path);
	if (obj == nullptr) {
		printf("Failed to load mesh: '%s', File doesn't exist\n", path);
		return;
	}

	m_verts = new Vertex[obj->num_triangles * 3];
	int num_verts = 0;

	for (int tri_idx = 0; tri_idx < obj->num_triangles; ++tri_idx) {
		Wavefront_Triangle triangle = obj->triangles[tri_idx];
		for (int vert_idx = 0; vert_idx < 3; ++vert_idx) {
			Wavefront_Vertex& vertex = triangle.vertices[vert_idx];
			Vertex new_vertex;
			new_vertex.position = obj->positions[vertex.idx_position];
			new_vertex.normal = obj->normals[vertex.idx_normal];
			new_vertex.texcoord = obj->texcoords[vertex.idx_texcoord];

			m_verts[num_verts++] = new_vertex;
		}
	}
	m_vert_count = num_verts;

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * num_verts, m_verts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (void*)(sizeof(glm::vec3)));

	m_draw_count = num_verts;
}