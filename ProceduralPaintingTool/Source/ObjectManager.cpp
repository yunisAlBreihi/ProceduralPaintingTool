#include "ObjectManager.h"

void ObjectManager::addShader(Shader shader)
{
	m_shaders.push_back(shader);
}

void ObjectManager::addMesh(Mesh mesh)
{
	m_meshes.push_back(mesh);
}

void ObjectManager::update()
{
	for (auto& mesh : m_meshes) {
		mesh.update();
	}
}

void ObjectManager::render()
{
	for (auto& mesh : m_meshes) {
		mesh.render();
	}
}
