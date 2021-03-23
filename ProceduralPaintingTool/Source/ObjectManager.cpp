#include "ObjectManager.h"
#include "Biome.h"

ObjectManager::ObjectManager(GLFWwindow* window) : m_window(window)
{
	Shader* t_shader = new Shader("Shaders/default.vs", "Shaders/default.fs");
	m_shaders.push_back(t_shader);

	m_terrain = new Mesh("Assets/Terrain.obj", Transform());

	m_camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f), m_window);
	m_timer = new Timer();
}

void ObjectManager::addShader(Shader* shader)
{
	m_shaders.push_back(shader);
}

void ObjectManager::addObject(BiomeObject* object)
{
	m_objects.push_back(object);
}

void ObjectManager::update()
{
	m_timer->update();
	m_camera->update(m_timer->deltaTime);

	RenderData t_renderData;
	t_renderData.m_eye_position = m_camera->getPosition();
	t_renderData.m_view_projection = m_camera->matrix();
	t_renderData.m_shader = m_shaders[0];
	t_renderData.m_directional_light = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));

	for (auto& t_object : m_objects) {
		t_renderData.m_model = t_object->m_mesh->getMatrix();
		mesh_draw(*t_object->m_mesh, t_renderData);
	}
	if (m_terrain != nullptr)
	{
		t_renderData.m_model = m_terrain->getMatrix();
		mesh_draw(*m_terrain, t_renderData);
	}
}

void ObjectManager::render()
{
	for (auto& mesh : m_objects) {
	}
}

void ObjectManager::clearMeshes()
{
	m_objects.clear();
}