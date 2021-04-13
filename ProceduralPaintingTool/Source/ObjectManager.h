#pragma once
#include <vector>
#include "Shader.h"
#include "Camera.h"
#include "Timer.h"
#include "Rendering.h"
#include "BiomeProperty.h"
#include "BiomeBrush.h"
#include "MousePicker.h"
#include "Debug.h"

class ObjectManager
{
private:
	std::vector<BiomeBrush*> m_objects;

	std::vector<Mesh*> m_debugMeshes;

	std::vector<DebugLine> m_debugLines;

	GLFWwindow* m_window = nullptr;
	Camera* m_camera = nullptr;
	Timer* m_timer = nullptr;
	MousePicker* m_mousePicker = nullptr;

	bool m_drawDebugLines = true;

	Shader* m_defaultShader = nullptr;
	Shader* m_debugShader = nullptr;

public:
	Mesh* m_terrain;

public:
	ObjectManager(GLFWwindow* window);

	void addObject(BiomeBrush* object);
	void addDebugMesh(Mesh* mesh);

	void update();

	void clearMeshes();
};