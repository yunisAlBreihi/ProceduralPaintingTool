#include "ObjectManager.h"
#include "Biome.h"
#include "GLFW/glfw3.h"
#include "Raycast.h"

ObjectManager::ObjectManager(GLFWwindow* window) : m_window(window)
{
	Shader* t_shader = new Shader("Shaders/default.vs", "Shaders/default.fs");
	m_shaders.push_back(t_shader);

	m_terrain = new Mesh("Assets/Terrain.obj", Transform());

	m_camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f), m_window);
	m_timer = new Timer();
	m_mousePicker = new MousePicker(m_camera);
}

void ObjectManager::addShader(Shader* shader)
{
	m_shaders.push_back(shader);
}

void ObjectManager::addObject(BiomeObject* object)
{
	m_objects.push_back(object);
}

void ObjectManager::addDebugMesh(Mesh* mesh)
{
	m_debugMeshes.push_back(mesh);
}

void ObjectManager::update()
{
	m_timer->update();
	m_camera->update(m_timer->deltaTime);

	//if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS)
	//{
	//	Transform t_transform;
	//	//t_transform.position = m_camera->get3Dpos();

	//	Mesh* t_mesh = new Mesh("Assets/Cube.obj", t_transform);
	//	addDebugMesh(t_mesh);
	//}

	RenderData t_renderData;
	t_renderData.m_eye_position = m_camera->getPosition();
	t_renderData.m_view_projection = m_camera->matrix();
	t_renderData.m_shader = m_shaders[0];
	t_renderData.m_directional_light = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));

	for (auto& t_object : m_objects) {
		t_renderData.m_model = t_object->m_mesh->getMatrix();
		mesh_draw(*t_object->m_mesh, t_renderData);
	}
	for (auto& t_debugMesh : m_debugMeshes)
	{
		t_renderData.m_model = t_debugMesh->getMatrix();
		mesh_draw(*t_debugMesh, t_renderData);
	}
	if (m_terrain != nullptr)
	{
		t_renderData.m_model = m_terrain->getMatrix();
		mesh_draw(*m_terrain, t_renderData);
	}

	m_mousePicker->update();
	if (globals::g_LMB_hold == true) {
		Vertex t_vertex = raycastFromCameraVertex(*m_camera, *m_mousePicker, *m_terrain, 50, 100.0f, 1.0f);
		m_terrain->setVertexColor(t_vertex.index, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	}
}

void ObjectManager::clearMeshes()
{
	m_objects.clear();
	m_debugMeshes.clear();
}