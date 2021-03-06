#pragma once
#include <vector>
#include "Shader.h"
#include "Camera.h"
#include "Timer.h"
#include "Rendering.h"
#include "BrushProperty.h"
#include "BiomeObject.h"
#include "MousePicker.h"
#include "Debug.h"
#include "BrushManager.h"

class Biome;

class ObjectManager {
private:
	std::vector<BiomeObject*> m_objects;
	std::vector<Mesh*> m_debugMeshes;
	std::vector<DebugLine> m_debugLines;

	BrushManager& m_brushManager;

	GLFWwindow* m_window = nullptr;
	Camera* m_camera = nullptr;
	Timer* m_timer = nullptr;
	MousePicker* m_mousePicker = nullptr;
	Biome* m_currentBiome = nullptr;

	bool m_drawDebugLines = true;

	Shader* m_defaultShader = nullptr;
	Shader* m_debugShader = nullptr;

public:
	Mesh* m_terrain = nullptr;

public:
	ObjectManager(GLFWwindow* window, BrushManager& brushManager);

	void addObject(BiomeObject* object);
	void addDebugMesh(Mesh* mesh);
	void start();
	void update();
	void quit();

	void clearMeshes();

	void createBiome();

	void fillTerrainColor(const glm::vec4& color);
	void fillTerrainColorCurrentBrush();
};