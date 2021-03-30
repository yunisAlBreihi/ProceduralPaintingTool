#include "GUIWindow.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Biome.h"

static char m_buffer[512];

GUIWindow::GUIWindow(Transform transform, ObjectManager& objectManager) : m_transform(transform), m_objectManager(objectManager)
{
	m_brushes.push_back(new Brush());
	m_brushes.push_back(new Brush());
	m_brushes.push_back(new Brush());

	m_currentBrush = m_brushes[0];
}

void GUIWindow::update()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//Create ImGUI window

	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	if (ImGui::CollapsingHeader("Biome1")) {
		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::SliderFloat2("Size Range", glm::value_ptr(m_currentBrush->m_sizeOffset), 0.1f, 10.0f);
		ImGui::SliderFloat2("Position Range X", glm::value_ptr(m_currentBrush->m_positionOffsetX), -10.0f, 10.0f);            // Edit 1 float3 using a slider from 0.0f to 1.0f
		ImGui::SliderFloat2("Position Range Y", glm::value_ptr(m_currentBrush->m_positionOffsetY), -10.0f, 10.0f);            // Edit 1 float3 using a slider from 0.0f to 1.0f
		ImGui::SliderFloat("Tree Radius", &m_currentBrush->m_radius, 0.1f, 10.0f);
		ImGui::SliderInt("Frequency", &m_currentBrush->m_frequency, 0, 20);
		ImGui::SliderInt("Priority", &m_currentBrush->m_priority, 0, 20);

		//listbox
		const char* t_items[] = { "Tree", "Bush", "Grass" };
		static int listbox_item_current = 0;
		ImGui::ListBox("Object type", &listbox_item_current, t_items, m_brushes.size());
		m_currentBrush = m_brushes[listbox_item_current];

		ImGui::SameLine();
		ImGui::Text("counter = %d", m_counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	if (ImGui::Button("Create Biome")) {	 // Buttons return true when clicked (most widgets return true when edited/activated)
		//Mesh* t_mesh = new Mesh("Assets/Monkey.obj", Transform(), m_objectManager.getShaders()[0]);
		//t_mesh->setPosition(glm::vec3(m_counter * 1.0f, 0.0f, 0.0f));
		//m_objectManager.addMesh(t_mesh);
		Biome(m_objectManager, m_brushes);
	}

	if (ImGui::Button("Clear Meshes")) {
		m_objectManager.clearMeshes();
	}
	
	ImGui::End();
}

void GUIWindow::render(GLFWwindow* window)
{
	ImGuiIO& t_io = ImGui::GetIO();
	int t_width, t_height;
	glfwGetWindowSize(window, &t_width, &t_height);
	t_io.DisplaySize = ImVec2(t_width, t_height);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (t_io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		GLFWwindow* t_backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(t_backup_current_context);
	}
}

void GUIWindow::updateSliders(Transform transform)
{
	m_transform = transform;
}
