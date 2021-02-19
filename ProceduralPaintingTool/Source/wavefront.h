#pragma once
#include <glm/glm.hpp>
struct Wavefront_Vertex
{
	int idx_position;
	int idx_texcoord;
	int idx_normal;
};

struct Wavefront_Triangle
{
	Wavefront_Vertex vertices[3];
};

struct Wavefront_File
{
	glm::vec3* positions;
	glm::vec2* texcoords;
	glm::vec3* normals;

	Wavefront_Triangle* triangles;
	int num_triangles;
};

Wavefront_File* wavefront_load(const char* path);
void wavefront_free(Wavefront_File* file);
