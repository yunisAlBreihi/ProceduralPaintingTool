#include "Mesh.h"
#include "wavefront.h"
#include "globals.h"

Mesh::Mesh(const char* path, Transform transform) : m_transform() {
	mesh_load(path);
	setPosition(transform.position);
	addRotation(0.0f, globals::UP);
	setScale(transform.scale);
}

void Mesh::setPosition(const glm::vec3& position) {
	m_transform.position = position;
	m_position_matrix = translate(glm::identity<glm::mat4>(), m_transform.position);
}

void Mesh::setRotation(const float& angle, const glm::vec3& axis) {
	m_transform.rotation.x = axis.x * glm::sin(angle * 0.5f);
	m_transform.rotation.y = axis.y * glm::sin(angle * 0.5f);
	m_transform.rotation.z = axis.z * glm::sin(angle * 0.5f);
	m_transform.rotation.w = glm::cos(angle * 0.5f);

	m_rotation_matrix = glm::rotate(glm::mat4(), glm::radians(angle), axis);
}

void Mesh::addRotation(const float& angle, const glm::vec3& axis) {
	m_transform.rotation.x = axis.x * glm::sin(angle * 0.5f);
	m_transform.rotation.y = axis.y * glm::sin(angle * 0.5f);
	m_transform.rotation.z = axis.z * glm::sin(angle * 0.5f);
	m_transform.rotation.w = glm::cos(angle * 0.5f);

	m_rotation_matrix = glm::rotate(m_rotation_matrix, glm::radians(angle), axis);
}

void Mesh::setVertexColor(int vertexIndex, const glm::vec4& color) {
	m_vertices[vertexIndex].color = color;
	update_bufferData();
}

void Mesh::setVertexColor(int vertexIndex, const float* color) {
	glm::vec4 t_color{ color[0], color[1], color[2], color[3] };
	m_vertices[vertexIndex].color = t_color;
	update_bufferData();
}

void Mesh::fillVertexColor(const glm::vec4& color) {
	for (size_t i = 0; i < m_vert_count; i++) {
		m_vertices[i].color = color;
		update_bufferData();
	}
}

void Mesh::fillVertexColor(const float* color) {
	glm::vec4 t_color{ color[0], color[1], color[2], color[4] };
	for (size_t i = 0; i < m_vert_count; i++) {
		m_vertices[i].color = t_color;
		update_bufferData();
	}
}

void Mesh::setScale(const glm::vec3& scale) {
	m_transform.scale = scale;
	m_scale_matrix = glm::scale(glm::mat4(1.0f), m_transform.scale);
}

const Vertex& Mesh::getVertexAtPosition(glm::vec3& targetPosition, float radius) {
	for (size_t i = 0; i < m_vert_count; i++) {
		if (targetPosition.x > m_vertices[i].position.x - radius &&
			targetPosition.x < m_vertices[i].position.x + radius &&
			targetPosition.y > m_vertices[i].position.y - radius &&
			targetPosition.y < m_vertices[i].position.y + radius &&
			targetPosition.z > m_vertices[i].position.z - radius &&
			targetPosition.z < m_vertices[i].position.z + radius) {
			return m_vertices[i];
		}
	}
	return Vertex{};
}

std::vector<Vertex*>& Mesh::getVertexAtPositionRadius(glm::vec3& targetPosition, float radius) {
	m_areaVertices.clear();
	for (size_t i = 0; i < m_vert_count; i++) {
		if (targetPosition.x > m_vertices[i].position.x - radius &&
			targetPosition.x < m_vertices[i].position.x + radius &&
			targetPosition.y > m_vertices[i].position.y - radius &&
			targetPosition.y < m_vertices[i].position.y + radius &&
			targetPosition.z > m_vertices[i].position.z - radius &&
			targetPosition.z < m_vertices[i].position.z + radius) {
			m_areaVertices.push_back(&m_vertices[i]);
		}
	}
	return m_areaVertices;
}

const Vertex& Mesh::getVertexAtPositionFlat(glm::vec3& targetPosition, float radius) {
	for (size_t i = 0; i < m_vert_count; i++) {
		if (targetPosition.x > m_vertices[i].position.x - radius &&
			targetPosition.x < m_vertices[i].position.x + radius &&
			targetPosition.z > m_vertices[i].position.z - radius &&
			targetPosition.z < m_vertices[i].position.z + radius) {
			return m_vertices[i];
		}
	}
	return Vertex{};
}

Vertex* Mesh::getVertexAtIndex(int index) {
	return &m_vertices[index];
}

const Vertex& Mesh::getLowestVertexPositionFlat() {
	Vertex* t_lowestVertex = nullptr;
	for (size_t i = 0; i < m_vert_count; i++) {
		if (t_lowestVertex == nullptr) {
			t_lowestVertex = &m_vertices[i];
			continue;
		}

		glm::ivec2 t_targetPositionInt{ static_cast<int>(m_vertices[i].position.x), static_cast<int>(m_vertices[i].position.z) };
		glm::ivec2 t_lowestPositionInt{ static_cast<int>(t_lowestVertex->position.x), static_cast<int>(t_lowestVertex->position.z) };

		if (t_targetPositionInt.x <= t_lowestPositionInt.x &&
			t_targetPositionInt.y <= t_lowestPositionInt.y) {
			t_lowestVertex = &m_vertices[i];
		}
	}
	return *t_lowestVertex;
}

const Vertex& Mesh::getHighestVertexPositionFlat() {
	Vertex* t_highestVertex = nullptr;
	for (size_t i = 0; i < m_vert_count; i++) {
		if (t_highestVertex == nullptr) {
			t_highestVertex = &m_vertices[i];
			continue;
		}

		glm::ivec2 t_targetPositionInt{ static_cast<int>(m_vertices[i].position.x), static_cast<int>(m_vertices[i].position.z) };
		glm::ivec2 t_highestPositionInt{ static_cast<int>(t_highestVertex->position.x), static_cast<int>(t_highestVertex->position.z) };

		if (t_targetPositionInt.x >= t_highestPositionInt.x &&
			t_targetPositionInt.y >= t_highestPositionInt.y) {
			t_highestVertex = &m_vertices[i];
		}
	}
	return *t_highestVertex;
}

void Mesh::mesh_load(const char* path) {
	Wavefront_File* obj = wavefront_load(path);
	if (obj == nullptr) {
		printf("Failed to load mesh: '%s', File doesn't exist\n", path);
		return;
	}

	m_vertices = new Vertex[obj->num_triangles * 3];
	int num_verts = 0;

	for (int tri_idx = 0; tri_idx < obj->num_triangles; ++tri_idx) {
		Wavefront_Triangle triangle = obj->triangles[tri_idx];
		for (int vert_idx = 0; vert_idx < 3; ++vert_idx) {
			Wavefront_Vertex& vertex = triangle.vertices[vert_idx];
			Vertex new_vertex;
			new_vertex.position = obj->positions[vertex.idx_position];
			new_vertex.normal = obj->normals[vertex.idx_normal];
			new_vertex.texcoord = obj->texcoords[vertex.idx_texcoord];
			new_vertex.color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			new_vertex.index = num_verts;
			m_vertices[num_verts++] = new_vertex;
		}
	}
	m_vert_count = num_verts;

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);

	update_bufferData();

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (void*)(sizeof(glm::vec3)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(Vertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3)));

	m_draw_count = num_verts;
}

void Mesh::update_bufferData()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vert_count, m_vertices, GL_STATIC_DRAW);
}
