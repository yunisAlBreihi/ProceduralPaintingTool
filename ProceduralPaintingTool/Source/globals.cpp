#include "globals.h"

namespace globals {
	float g_brush_radius = 2.0f;
	int g_seed = 1;
	bool g_cam_control = false;
	bool g_LMB_hold = false;
	glm::vec2 g_mouse_position;
	glm::vec2 g_mouse_delta;
	glm::vec2 g_window_size;

	//Save file variables
	bool g_hasBrushSave = false;
	bool g_hasGlobalsSave = false;
	bool g_hasTerrainVerticesSave = false;
	const char* g_saveNameBrushes = "SaveFiles/brushes.json";
	const char* g_saveNameGlobals = "SaveFiles/globals.json";
	const char* g_saveNameTerrainVertices = "SaveFiles/terrainVertices.json";

	//For brush save files
	const char* g_brushPrefix = "Brush";
	const char* g_propertyPrefix = "Property";

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

	float randRangef(float min, float max) {
		int t_scaledMin = static_cast<int>(min * 100);
		int t_scaledMax = static_cast<int>(max * 100);

		if (glm::abs(t_scaledMax) + glm::abs(t_scaledMin) == 0) {
			return 0.0f;
		}
		return (rand() % t_scaledMax + t_scaledMin) * 0.01f;
	}

	float randRangef(glm::vec2 value) {
		int t_scaledMin = static_cast<int>(value.x * 100);
		int t_scaledMax = static_cast<int>(value.y * 100);

		if (glm::abs(t_scaledMax) + glm::abs(t_scaledMin) == 0) {
			return 0.0f;
		}
		return (rand() % t_scaledMax + t_scaledMin) * 0.01f;
	}
}