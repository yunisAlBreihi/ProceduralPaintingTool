#include "ObjectManager.h"

ObjectManager::ObjectManager(GLFWwindow* window) : m_window(window)
{
	Shader* t_shader = new Shader("Shaders/default.vs", "Shaders/default.fs");
	m_shaders.push_back(t_shader);

	for (size_t x = 0; x < 10; ++x) {
		for (size_t z = 0; z < 10; ++z) {
			const float t_randomSize = (glm::min((rand() % 100 + 50), 100) * 0.01f);
			const float t_offsetX = (rand() % 200 - 100) * 0.01f;
			const float t_offsetZ = (rand() % 200 - 100) * 0.01f;
			const glm::vec3 t_treePos = glm::vec3(x * 3 + t_offsetX, 0.0f, z * 3 + t_offsetZ);

			Mesh* t_mesh = new Mesh("Assets/Tree.obj", Transform());
			t_mesh->setPosition(t_treePos);
			t_mesh->setScale(glm::vec3(t_randomSize, t_randomSize, t_randomSize));
			m_meshes.push_back(t_mesh);
		}
	}

	//Mesh* t_mesh = new Mesh("Assets/Monkey.obj", Transform());
	//t_mesh->setPosition(glm::vec3(-2.5f, 0.0f, 0.0f));
	//m_meshes.push_back(t_mesh);

	//t_mesh = new Mesh("Assets/Monkey.obj", Transform());
	//t_mesh->setPosition(glm::vec3(2.5f, 0.0f, 0.0f));
	//m_meshes.push_back(t_mesh);


	m_camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f), m_window);
	m_timer = new Timer();
}

void ObjectManager::addShader(Shader* shader)
{
	m_shaders.push_back(shader);
}

void ObjectManager::addMesh(Mesh* mesh)
{
	m_meshes.push_back(mesh);
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

	for (auto& t_mesh : m_meshes) {
		//mesh->update();
		//t_renderData.m_model = glm::translate(glm::identity<glm::mat4>(), glm::vec3(-2.5f, 0.0f, 0.0f));
		//t_mesh->setPosition(glm::vec3(0.0f, 5.0f, 0.0f));
		t_renderData.m_model = t_mesh->getMatrix();
		mesh_draw(*t_mesh, t_renderData);
	}
}

void ObjectManager::render()
{
	for (auto& mesh : m_meshes) {
	}
}
