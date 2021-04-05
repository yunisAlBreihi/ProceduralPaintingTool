#pragma once
#include <vector>
#include "Shader.h"
#include "Camera.h"
#include "Timer.h"
#include "Rendering.h"
#include "Brush.h"
#include "BiomeObject.h"
#include "MousePicker.h"

class ObjectManager
{
private:
	std::vector<Shader*> m_shaders;
	std::vector<BiomeObject*> m_objects;

	std::vector<Mesh*> m_debugMeshes;

	GLFWwindow* m_window;
	Camera* m_camera;
	Timer* m_timer;
	MousePicker* m_mousePicker;

public:
	Mesh* m_terrain;

public:
	ObjectManager(GLFWwindow* window);

	void addShader(Shader* shader);
	void addObject(BiomeObject* object);
	void addDebugMesh(Mesh* mesh);

	void update();

	void clearMeshes();
};