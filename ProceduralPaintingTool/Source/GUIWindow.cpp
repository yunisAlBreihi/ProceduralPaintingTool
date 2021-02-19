#include "GUIWindow.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

static char m_buffer[512];

void GUIWindow::update()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//Create ImGUI window
	static int counter = 0;

	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	ImGui::SliderFloat3("Position", glm::value_ptr(m_transform.position), -10.0f, 10.0f);            // Edit 1 float3 using a slider from 0.0f to 1.0f
	ImGui::SliderFloat3("Rotation", glm::value_ptr(m_transform.rotation), -180.0f, 180.0f);            // Edit 1 float3 using a slider from 0.0f to 1.0f
	ImGui::SliderFloat3("Scale", glm::value_ptr(m_transform.scale), -10.0f, 10.0f);            // Edit 1 float3 using a slider from 0.0f to 1.0f

	if(ImGui::InputText("Import Mesh", m_buffer, 512, ImGuiInputTextFlags_EnterReturnsTrue)) {
		Mesh t_mesh(m_buffer,Transform(), m_objectManager.getShaders()[0]);
		m_transform = t_mesh.getTransform();
		m_objectManager.addMesh(t_mesh);
		printf("hej");
	}
	if (ImGui::Button("Button")) {	 // Buttons return true when clicked (most widgets return true when edited/activated)
		counter++;
	}
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
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

void GUIWindow::updateSliders(Transform& transform)
{
	m_transform = transform;
}
