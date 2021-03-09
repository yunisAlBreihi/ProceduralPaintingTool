#pragma once
#include <vector>
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Timer.h"
#include "Rendering.h"

class ObjectManager
{
private:
	std::vector<Shader*> m_shaders;
	std::vector<Mesh*> m_meshes;

	GLFWwindow* m_window;
	Camera* m_camera;
	Timer* m_timer;


public:
	ObjectManager(GLFWwindow* window);

	void addShader(Shader* shader);
	void addMesh(Mesh* mesh);

	void update();
	void render();
};