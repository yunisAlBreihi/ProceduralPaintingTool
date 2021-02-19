#pragma once
#include <vector>
#include "Mesh.h"
#include "Shader.h"

class ObjectManager
{
private:
	std::vector<Shader> m_shaders;
	std::vector<Mesh> m_meshes;

public:
	void addShader(Shader shader);
	void addMesh(Mesh mesh);

	std::vector<Shader> getShaders() { return m_shaders; }
	std::vector<Mesh> getMeshes() { return m_meshes; }

	void update();
	void render();
};