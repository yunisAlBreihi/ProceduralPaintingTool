#include "GUIWindow.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Biome.h"

static char m_buffer[512];

void GUIWindow::update()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//Create ImGUI window

	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	if (ImGui::CollapsingHeader("Biome1")) {
		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::SliderFloat2("Size Range", glm::value_ptr(m_brush.m_sizeOffset), 0.1f, 10.0f);
		//ImGui::SliderFloat("Size Range X", m_brush.m_sizeRange.x, -10.0f, 10.0f);            // Edit 1 float3 using a slider from 0.0f to 1.0f
		ImGui::SliderFloat2("Position Range X", glm::value_ptr(m_brush.m_positionOffsetX), -10.0f, 10.0f);            // Edit 1 float3 using a slider from 0.0f to 1.0f
		ImGui::SliderFloat2("Position Range Y", glm::value_ptr(m_brush.m_positionOffsetY), -10.0f, 10.0f);            // Edit 1 float3 using a slider from 0.0f to 1.0f
		ImGui::SliderFloat("Tree Radius", &m_brush.m_treeRadius, 0.1f, 10.0f);            // Edit 1 float3 using a slider from 0.0f to 1.0f

		if (ImGui::InputText("Import Mesh", m_buffer, 512, ImGuiInputTextFlags_EnterReturnsTrue)) {
			//Mesh* t_mesh = new Mesh(m_buffer,Transform(), m_objectManager.getShaders()[0]);
			//m_transform = t_mesh->getTransform();
			//m_objectManager.addMesh(t_mesh);
		}

		ImGui::SameLine();
		ImGui::Text("counter = %d", m_counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	if (ImGui::Button("Button")) {	 // Buttons return true when clicked (most widgets return true when edited/activated)
		m_counter++;
		//Mesh* t_mesh = new Mesh("Assets/Monkey.obj", Transform(), m_objectManager.getShaders()[0]);
		//t_mesh->setPosition(glm::vec3(m_counter * 1.0f, 0.0f, 0.0f));
		//m_objectManager.addMesh(t_mesh);
		Biome(m_objectManager, m_brush);
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
