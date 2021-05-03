#include "ObjectManager.h"
#include "Biome.h"
#include "GLFW/glfw3.h"
#include "Raycast.h"
#include <algorithm>
#include "IOHandler.h"

ObjectManager::ObjectManager(GLFWwindow* window, BrushManager& brushManager) : m_window(window), m_brushManager(brushManager) {
	m_defaultShader = new Shader("Shaders/default.vs", "Shaders/default.fs");
	m_debugShader = new Shader("Shaders/Debug.vs", "Shaders/Debug.fs");

	m_terrain = new Mesh("Assets/Terrain.obj", Transform());

	m_camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f), m_window);
	m_timer = new Timer();
	m_mousePicker = new MousePicker(m_camera);
}

void ObjectManager::addObject(BiomeObject* object) {
	m_objects.push_back(object);
}

void ObjectManager::addDebugMesh(Mesh* mesh) {
	m_debugMeshes.push_back(mesh);
}

void ObjectManager::start() {
	IOHandler::loadJson_terrainVerticesColor(globals::g_saveNameTerrainVertices, *this, m_brushManager);
	Biome(*this, m_brushManager);
}

void ObjectManager::update() {
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
	for (auto& t_debugMesh : m_debugMeshes)	{
		t_mesh_RD.m_model = t_debugMesh->getMatrix();
		mesh_draw(*t_debugMesh, t_mesh_RD);
	}
	if (m_terrain != nullptr) {
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
	m_mousePicker->update();
	if (globals::g_LMB_hold == true) {
		std::vector<Vertex*>& t_vertices = raycastFromCameraVertexRadius(*m_camera, *m_mousePicker, *m_terrain, 50, 100.0f, globals::g_brush_radius);

		for (auto& t_vertex : t_vertices) {
			if (m_brushManager.getCurrentBrush() != nullptr) {
				if (m_brushManager.getCurrentBrush()->m_id != 0) {
					m_brushManager.getCurrentBrush()->addVertex(t_vertex);
				}
				m_terrain->setVertexColor(t_vertex->index, m_brushManager.getCurrentBrush()->m_vertexColor);
				m_brushManager.removeVertexFromOtherBrushesAtPosition(t_vertex->position);
			}
		}
	}
}

void ObjectManager::quit() {
	IOHandler::saveJson_terrainVerticesColor(globals::g_saveNameTerrainVertices, *this);
}

void ObjectManager::clearMeshes() {
	m_objects.clear();
	m_debugMeshes.clear();
}

void ObjectManager::fillTerrainColor(const glm::vec4& color) {
	m_terrain->fillVertexColor(color);
	if (color == globals::EMPTY_COLOR_V) {
		m_brushManager.removeVerticesFromAllBrushes();
	}
	else {
		Vertex* t_terrainVertices = m_terrain->m_vertices;
		int t_terrainVertexSize = m_terrain->m_vert_count;

		for (size_t i = 0; i < t_terrainVertexSize; ++i) {
			m_brushManager.getCurrentBrush()->addVertex(&t_terrainVertices[i]);
			m_brushManager.removeVerticesFromOtherBrushes();
		}

	}
}

void ObjectManager::fillTerrainColorCurrentBrush() {
	Vertex* t_terrainVertices = m_terrain->m_vertices;
	for (size_t i = 0; i < m_terrain->m_vert_count; ++i) {
		if (m_brushManager.getCurrentBrush() != nullptr) {
			m_brushManager.getCurrentBrush()->addVertex(&t_terrainVertices[i]);
		}
	}
	m_terrain->fillVertexColor(m_brushManager.getCurrentBrush()->m_vertexColor);
	m_brushManager.removeVerticesFromOtherBrushes();
}
