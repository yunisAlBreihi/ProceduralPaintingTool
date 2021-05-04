#pragma once
#include <glm/glm.hpp>
#include "Vertex.h"
#include <vector>

namespace globals
{
	static const glm::vec3 RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);
	static const glm::vec3 LEFT = glm::vec3(-1.0f, 0.0f, 0.0f);
	static const glm::vec3 FORWARD = glm::vec3(0.0f, 0.0f, -1.0f);
	static const glm::vec3 BACK = glm::vec3(0.0f, 0.0f, 1.0f);
	static const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
	static const glm::vec3 ZERO = glm::vec3(0.0f);
	static const glm::vec3 ONE = glm::vec3(1.0f);
	static const glm::vec3 MINUS_ONE = glm::vec3(-1.0f);
	static const glm::vec4 EMPTY_COLOR_V = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	static const float MAX_CAM_PITCH = 1.57f;
	static const float MIN_CAM_PITCH = -1.57f;
	static const float FLOAT_ERROR_THRESHOLD = 0.001f;
	static const char* VIEWPROJECTION = "u_ViewProjection";

	extern float g_brush_radius;
	extern int g_seed;
	extern bool g_cam_control;
	extern bool g_LMB_hold;
	extern glm::vec2 g_mouse_position;
	extern glm::vec2 g_mouse_delta;
	extern glm::ivec2 g_window_size;

	//Save file variables
	extern bool g_hasBrushSave;
	extern bool g_hasGlobalsSave;
	extern bool g_hasTerrainVerticesSave;
	extern const char* g_saveNameBrushes;
	extern const char* g_saveNameGlobals;
	extern const char* g_saveNameTerrainVertices;

	//For naming of brush save files
	extern const char* g_brushPrefix;
	extern const char* g_propertyPrefix;

	void printVec3(const glm::vec3& vec3);

	Vertex* getVertexAtPositionFlat(const std::vector<Vertex*>& vertexList,const glm::vec3& position, float radius);

	float randRangef(float min, float max);
	float randRangef(glm::vec2 value);
};