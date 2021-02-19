#include "wavefront.h"
#include <fstream>
using namespace glm;
using namespace std;

static vec3 parse_vec3(const char* line)
{
	vec3 result;

	// Offset by 2 to skip 'v', 'vf', or 'vn'
	sscanf_s(line + 2, "%f %f %f", &result.x, &result.y, &result.z);
	return result;
}

static vec2 parse_vec2(const char* line)
{
	vec2 result;

	// Offset by 2 to skip 'v', 'vf', or 'vn'
	sscanf_s(line + 2, "%f %f", &result.x, &result.y);
	return result;
}

static bool parse_vertex_and_jump(char** str, Wavefront_Vertex* vert)
{
	// nullptr or null character
	if (!(*str) || !(**str))
		return false;

	if (sscanf_s(*str, "%d/%d/%d", &vert->idx_position, &vert->idx_texcoord, &vert->idx_normal) == EOF)
		return false;

	// 1 to 0-based...
	vert->idx_position -= 1;
	vert->idx_texcoord -= 1;
	vert->idx_normal -= 1;

	// Jump to next spacer
	*str = strstr(*str + 1, " ");
	return true;
}

Wavefront_File* wavefront_load(const char* path)
{
	// Open file
	ifstream file;
	file.open(path);

	if (!file)
		return NULL;

	Wavefront_File* wav = new Wavefront_File();

	// Count positions, texcoords etc...
	int num_positions = 0;
	int num_texcoords = 0;
	int num_normals = 0;
	int num_triangles = 0;

	static char buffer[512];
	while(file.getline(buffer, 512))
	{
		// Position
		if (strncmp(buffer, "v ", 2) == 0)
			num_positions++;
		// Texture coordinate
		else if (strncmp(buffer, "vt", 2) == 0)
			num_texcoords++;
		// Normal
		else if (strncmp(buffer, "vn", 2) == 0)
			num_normals++;

		// Face
		else if (strncmp(buffer, "f ", 2) == 0)
		{
			// Faces are rough, since we need to triangulate for OpenGL
			// So every face with verts > 3 turns into 1 + (verts - 3) triangles (using TRIANGLE_FAN method)
			int verts_in_face = 0;
			char* ptr = buffer + 2;

			Wavefront_Vertex dummy; // :I
			while(parse_vertex_and_jump(&ptr, &dummy))
				verts_in_face++;

			num_triangles += 1 + (verts_in_face - 3);
		}
	}

	// Allocate everything
	wav->positions = new vec3[num_positions];
	wav->texcoords = new vec2[num_texcoords];
	wav->normals = new vec3[num_normals];
	wav->triangles = new Wavefront_Triangle[num_triangles];
	wav->num_triangles = num_triangles;
 
	// Reset, time to parse this puppy!
	num_positions = num_texcoords = num_normals = num_triangles = 0;

	file.clear();
	file.seekg(ios_base::beg);

	while(file.getline(buffer, 512))
	{
		// Position
		if (strncmp(buffer, "v ", 2) == 0)
		{
			vec3 pos;
			sscanf_s(buffer, "v %f %f %f", &pos.x, &pos.y, &pos.z);
			wav->positions[num_positions++] = pos;
		}
		// Texture coordinate
		else if (strncmp(buffer, "vt", 2) == 0)
		{
			vec2 texcoord;
			sscanf_s(buffer, "vt %f %f", &texcoord.x, &texcoord.y);
			wav->texcoords[num_texcoords++] = texcoord;
		}
		// Normal
		else if (strncmp(buffer, "vn", 2) == 0)
		{
			vec3 normal;
			sscanf_s(buffer, "vn %f %f %f", &normal.x, &normal.y, &normal.z);
			wav->normals[num_normals++] = normal;
		}
		// Face
		else if (strncmp(buffer, "f ", 2) == 0)
		{
			char* ptr = buffer + 2;

			// Load two first verts for TRIANGLE_FAN'ning
			Wavefront_Vertex first_vert;
			Wavefront_Vertex prev_vert;

			parse_vertex_and_jump(&ptr, &first_vert);
			parse_vertex_and_jump(&ptr, &prev_vert);

			// Then move forwards one at a time
			Wavefront_Vertex next_vert;
			while(parse_vertex_and_jump(&ptr, &next_vert))
			{
				Wavefront_Triangle tri;
				tri.vertices[0] = first_vert;
				tri.vertices[1] = prev_vert;
				tri.vertices[2] = next_vert;

				wav->triangles[num_triangles++] = tri;

				// This vert now becomes the 'prev' vert, but first will stay constant
				prev_vert = next_vert;

				// I swear it took every strength in me to not accidentally write "perv_vert"
				// I'm sorry jesus
			}
		}
	}

	// Done :)
	return wav;
}

void wavefront_free(Wavefront_File* file)
{
	if (!file)
		return;

	delete file->positions;
	delete file->texcoords;
	delete file->normals;
	delete file->triangles;

	delete file;
}