#pragma once
#include <vector>
#include "Shader.h"
#include "Camera.h"
#include "Timer.h"
#include "Rendering.h"
#include "Brush.h"
#include "BiomeObject.h"

class ObjectManager
{
private:
	std::vector<Shader*> m_shaders;

	GLFWwindow* m_window;
	Camera* m_camera;
	Timer* m_timer;

public:
	std::vector<BiomeObject*> m_objects;
	Mesh* m_terrain;

public:
	ObjectManager(GLFWwindow* window);

	void addShader(Shader* shader);
	void addObject(BiomeObject* object);

	void update();
	void render();

	void clearMeshes();
};