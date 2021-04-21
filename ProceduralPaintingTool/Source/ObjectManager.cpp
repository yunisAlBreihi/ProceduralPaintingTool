#include "ObjectManager.h"
#include "Biome.h"
#include "GLFW/glfw3.h"
#include "Raycast.h"
#include <algorithm>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using value_t = nlohmann::detail::value_t;

struct Json_File
{
public:
	std::pair<const char*, int> m_int = { "int", 1 };
	std::pair<const char*, float> m_float = { "float", 1.546f };
	std::pair<const char*, std::string> m_string = { "string", "Pekka" };
};

struct Json_Object
{
public:
	std::vector<Json_File*> m_jsonFiles;

	Json_Object(const int length) {
		for (size_t i = 0; i < length; ++i) {
			Json_File* t_file = new Json_File();
			t_file->m_int.second *= i;
			t_file->m_float.second *= i;
			std::string t_nameIndex = std::to_string(i);
			t_file->m_string.second += t_nameIndex;
			m_jsonFiles.push_back(t_file);
		}
	}
};

ObjectManager::ObjectManager(GLFWwindow* window, BrushManager& brushManager) : m_window(window), m_brushManager(brushManager) {
	m_defaultShader = new Shader("Shaders/default.vs", "Shaders/default.fs");
	m_debugShader = new Shader("Shaders/Debug.vs", "Shaders/Debug.fs");

	m_terrain = new Mesh("Assets/Terrain.obj", Transform());

	m_camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f), m_window);
	m_timer = new Timer();
	m_mousePicker = new MousePicker(m_camera);







	Json_Object* t_jsonObject = new Json_Object(5);

	json t_json3;
	for (size_t i = 0; i < t_jsonObject->m_jsonFiles.size(); i++) {
		std::string t_itemName = "item" + std::to_string(i);
		t_json3[t_itemName] = { t_jsonObject->m_jsonFiles[i]->m_float, t_jsonObject->m_jsonFiles[i]->m_int, t_jsonObject->m_jsonFiles[i]->m_string };
	}

	std::ofstream t_o("testfile.json");
	t_o << t_json3 << std::endl;

	std::ifstream t_i("testfile.json");
	json t_jsonIn;
	t_i >> t_jsonIn;

	for (const auto& t_item : t_jsonIn) {
		std::cout << t_item << std::endl;
	}
}

void ObjectManager::addObject(BiomeBrush* object) {
	m_objects.push_back(object);
}

void ObjectManager::addDebugMesh(Mesh* mesh) {
	m_debugMeshes.push_back(mesh);
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
	m_mousePicker->update();
	if (globals::g_LMB_hold == true) {
		std::vector<Vertex*>& t_vertices = raycastFromCameraVertexRadius(*m_camera, *m_mousePicker, *m_terrain, 50, 100.0f, 2.0f);

		for (auto& t_vertex : t_vertices) {
			if (m_brushManager.getCurrentBrush() != nullptr) {
				m_brushManager.getCurrentBrush()->addVertex(t_vertex);
				m_terrain->setVertexColor(t_vertex->index, m_brushManager.getCurrentBrush()->m_vertexColor);
				m_brushManager.removeVertexFromOtherBrushesAtPosition(t_vertex->position);
			}
		}
	}
}

void ObjectManager::clearMeshes() {
	m_objects.clear();
	m_debugMeshes.clear();
}

void ObjectManager::fillTerrainColor(const glm::vec4& color) {
	m_terrain->fillVertexColor(color);
}

void ObjectManager::fillTerrainColor(const float* color) {
	m_terrain->fillVertexColor(color);
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
