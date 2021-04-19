#include "globals.h"

namespace globals 
{
	bool g_cam_control = false;
	bool g_LMB_hold = false;
	glm::vec2 g_mouse_position;
	glm::vec2 g_mouse_delta;
	glm::vec2 g_window_size;

	void printVec3(const glm::vec3& vector) {
		printf("X: %0.01f, Y: %0.01f, Z: %0.01f\n", vector.x, vector.y, vector.z);
	}

	Vertex* getVertexAtPositionFlat(const std::vector<Vertex*>& vertices, const glm::vec3& position, float radius) {
		int t_arraySize = vertices.size();

		for (size_t i = 0; i < t_arraySize; i++) {
			if (vertices[i] != nullptr) {
				if (position.x > vertices[i]->position.x - radius &&
					position.x < vertices[i]->position.x + radius &&
					position.z > vertices[i]->position.z - radius &&
					position.z < vertices[i]->position.z + radius) {
					return vertices[i];
				}
			}
		}
		return nullptr;
	}
}