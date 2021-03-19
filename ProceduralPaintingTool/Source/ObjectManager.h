#pragma once
#include <vector>
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Timer.h"
#include "Rendering.h"
#include "Brush.h"

class ObjectManager
{
private:
	std::vector<Shader*> m_shaders;

	GLFWwindow* m_window;
	Camera* m_camera;
	Timer* m_timer;

public:
	std::vector<Mesh*> m_meshes;

public:
	ObjectManager(GLFWwindow* window);

	void addShader(Shader* shader);
	void addMesh(Mesh* mesh);

	void update();
	void render();

	void createBiome(Brush brush);
};