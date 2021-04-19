#include "ObjectManager.h"
#include "Biome.h"
#include "GLFW/glfw3.h"
#include "Raycast.h"
#include <algorithm>

ObjectManager::ObjectManager(GLFWwindow* window, BrushManager& brushManager) : m_window(window), m_brushManager(brushManager) {
	m_defaultShader = new Shader("Shaders/default.vs", "Shaders/default.fs");
	m_debugShader = new Shader("Shaders/Debug.vs", "Shaders/Debug.fs");

	m_terrain = new Mesh("Assets/Terrain.obj", Transform());

	m_camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f), m_window);
	m_timer = new Timer();
	m_mousePicker = new MousePicker(m_camera);
}

void ObjectManager::addObject(BiomeBrush* object)
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

	//Draw meshes
	RenderData t_mesh_RD;
	t_mesh_RD.m_eye_position = m_camera->getPosition();
	t_mesh_RD.m_view_projection = m_camera->matrix();
	t_mesh_RD.m_shader = m_defaultShader;
	t_mesh_RD.m_directional_light = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));

	for (auto& t_object : m_objects) {
		t_mesh_RD.m_model = t_object->m_mesh->getMatrix();
		mesh_draw(*t_object->m_mesh, t_mesh_RD);
	}
	for (auto& t_debugMesh : m_debugMeshes)
	{
		t_mesh_RD.m_model = t_debugMesh->getMatrix();
		mesh_draw(*t_debugMesh, t_mesh_RD);
	}
	if (m_terrain != nullptr)
	{
		t_mesh_RD.m_model = m_terrain->getMatrix();
		mesh_draw(*m_terrain, t_mesh_RD);
	}

	//Draw debug lines
	RenderData t_debugLine_RD;
	t_debugLine_RD.m_shader = m_debugShader;

	for (auto& t_debugLine : m_debugLines) {
		if (t_debugLine.active()) {
			debugLine_draw(t_debugLine, t_mesh_RD);
			t_debugLine.update(m_timer->deltaTime);
		}
	}
	static int t_counter = 0;
	m_mousePicker->update();
	if (globals::g_LMB_hold == true) {
		std::vector<Vertex*>& t_vertices = raycastFromCameraVertexRadius(*m_camera, *m_mousePicker, *m_terrain, 50, 100.0f, 2.0f);

		for (auto& t_vertex : t_vertices) {
			//printf("X: %0.1f, Z: %0.1f\n", t_vertex->position.x, t_vertex->position.z);
			if (m_brushManager.getCurrentBrush() != nullptr) {
				m_brushManager.getCurrentBrush()->addVertex(t_vertex);
				m_terrain->setVertexColor(t_vertex->index, m_brushManager.getCurrentBrush()->m_vertexColor);
				m_brushManager.removeVertexFromOtherBrushesAtPosition(t_vertex->position);
			}
		}
	}
}

void ObjectManager::clearMeshes()
{
	m_objects.clear();
	m_debugMeshes.clear();
}