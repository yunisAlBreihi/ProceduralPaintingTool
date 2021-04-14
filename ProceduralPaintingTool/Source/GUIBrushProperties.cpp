#include "GUIBrushProperties.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Biome.h"


GUIBrushProperties::GUIBrushProperties(ObjectManager& objectManager, BrushManager& brushManager) : 
	m_objectManager(objectManager), m_brushManager(brushManager) {}

void GUIBrushProperties::update() {

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//Create ImGUI window
	ImGui::Begin("Brush Properties");

	auto& t_brushes = m_brushManager.getBrushes();
	BrushProperty* t_currentBrush = m_brushManager.getCurrentBrush();

	for (int i = 0; i < t_brushes.size(); ++i) {
		std::string brushName = "Brush" + std::to_string(i);
		if (ImGui::Button(brushName.c_str())) {
			m_brushManager.setCurrentBrush(i);
			if (t_currentBrush->getObjectPropertiesLength() != 0) {
				m_currentObjectProperty = &t_currentBrush->getCurrentObjectType();
			}
			else {
				t_currentBrush->m_current_object = -1;
			}
		}
		ImGui::SameLine();
	}

	if (ImGui::Button("Create Brush")) {	 // Buttons return true when clicked (most widgets return true when edited/activated)
		m_brushManager.CreateBrush();
	}

	//ImGui::ColorPicker4("Brush Vertex Color", (float*)&m_currentBrushProperty->m_vertexColor, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
	if (ImGui::CollapsingHeader("Brush Color")) {
		if (t_currentBrush != nullptr) {
			ImGui::ColorPicker4("Brush Vertex Color", (float*)&t_currentBrush->m_vertexColor);
		}
	}

	if (ImGui::CollapsingHeader("Biome Properties")) {
		if (t_currentBrush != nullptr) {
			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			if (t_currentBrush->m_current_object != -1) {
				ImGui::SliderFloat2("Size Range", glm::value_ptr(m_currentObjectProperty->m_sizeOffset), 0.1f, 10.0f);
				ImGui::SliderFloat2("Position Range X", glm::value_ptr(m_currentObjectProperty->m_positionOffsetX), -10.0f, 10.0f);            // Edit 1 float3 using a slider from 0.0f to 1.0f
				ImGui::SliderFloat2("Position Range Y", glm::value_ptr(m_currentObjectProperty->m_positionOffsetY), -10.0f, 10.0f);            // Edit 1 float3 using a slider from 0.0f to 1.0f
				ImGui::SliderFloat("Tree Radius", &m_currentObjectProperty->m_radius, 0.1f, 10.0f);
				ImGui::SliderInt("Frequency", &m_currentObjectProperty->m_frequency, 0, 20);
				ImGui::SliderInt("Priority", &m_currentObjectProperty->m_priority, 0, 20);
			}

			//listbox
			if (t_currentBrush->getObjectPropertiesLength() != 0) {
				static int t_currentItem = 0;
				ImGui::ListBox("Object type", &t_currentItem, t_currentBrush->getObjectNames_C().data(), t_currentBrush->getObjectPropertiesLength());
				m_currentObjectProperty = t_currentBrush->m_objectProperties[t_currentItem];
			}
			ImGui::SameLine();

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			if (ImGui::InputText("Import Mesh", m_buffer, 512, ImGuiInputTextFlags_EnterReturnsTrue)) {
				t_currentBrush->addObjectType(new BiomeObjectProperty());
				t_currentBrush->getCurrentObjectType().m_name = m_buffer;
				m_currentObjectProperty = &t_currentBrush->getCurrentObjectType();
			}
		}
	}

	if (ImGui::Button("Create Biome")) {	 // Buttons return true when clicked (most widgets return true when edited/activated)
		Biome(m_objectManager, m_brushManager);
	}

	if (ImGui::Button("Clear Meshes")) {
		m_objectManager.clearMeshes();
	}

	ImGui::End();
}

void GUIBrushProperties::render(GLFWwindow* window) {
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