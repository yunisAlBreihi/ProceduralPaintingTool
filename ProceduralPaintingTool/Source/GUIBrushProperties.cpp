#include "GUIBrushProperties.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Biome.h"
#include "imgui/ImFileDialog.h"

GUIBrushProperties::GUIBrushProperties(ObjectManager& objectManager, BrushManager& brushManager) :
	m_objectManager(objectManager), m_brushManager(brushManager) {
}

void GUIBrushProperties::start() {
	ifd::FileDialog::Instance().CreateTexture = [](uint8_t* data, int w, int h, char fmt) -> void* {
		GLuint tex;

		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, (fmt == 0) ? GL_BGRA : GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		return (void*)tex;
	};
	ifd::FileDialog::Instance().DeleteTexture = [](void* tex) {
		GLuint texID = (GLuint)tex;
		glDeleteTextures(1, &texID);
	};
}

void GUIBrushProperties::update() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//Create ImGUI window
	ImGui::Begin("Brush Properties");


	static int t_seed = 0;
	ImGui::InputInt("Seed", &globals::g_seed, ImGuiInputTextFlags_EnterReturnsTrue);

	ImGui::Separator();

	auto& t_brushes = m_brushManager.getBrushes();

	BrushProperty* t_currentBrush = m_brushManager.getCurrentBrush();
	if (t_currentBrush != nullptr && t_currentBrush->m_current_object != -1 && m_currentObjectProperty == nullptr) {
		m_currentObjectProperty = &t_currentBrush->getCurrentObjectType();
	}

	static int t_currentItem = 0;
	for (int i = 0; i < t_brushes.size(); ++i) {
		if (ImGui::Button(t_brushes[i]->m_name.c_str())) {
			m_brushManager.setCurrentBrush(i);
			t_currentItem = 0;
			if (t_currentBrush->getObjectPropertiesLength() != 0) {
				m_currentObjectProperty = &t_currentBrush->getCurrentObjectType();
			}
			else {
				t_currentBrush->m_current_object = -1;
			}
		}
		ImGui::SameLine();
	}

	ImGui::Separator();

	if (ImGui::Button("Create Brush")) {	 // Buttons return true when clicked (most widgets return true when edited/activated)
		ImGui::OpenPopup("brush_naming_popup");
	}
	if (ImGui::BeginPopup("brush_naming_popup")) {
		if (ImGui::InputText("Brush Name", m_buffer, 512, ImGuiInputTextFlags_EnterReturnsTrue)) {
			printf(m_buffer);
			ImGui::CloseCurrentPopup();
			m_brushManager.createBrush(m_buffer);
		}
		ImGui::EndPopup();
	}

	if (t_currentBrush != nullptr && t_currentBrush->m_id != 0 && t_currentBrush->m_colorIsSet == false) {
		if (ImGui::CollapsingHeader("Brush Color")) {
			ImGui::ColorPicker4("Brush Vertex Color", (float*)&t_currentBrush->m_vertexColor);
		}
	}

	if (t_currentBrush != nullptr && t_currentBrush->m_id != 0) {
		if (ImGui::Button("Fill terrain with color")) {	 // Buttons return true when clicked (most widgets return true when edited/activated)
			m_objectManager.fillTerrainColorCurrentBrush();
		}

		ImGui::Separator();

		if (ImGui::CollapsingHeader("Biome Properties")) {
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
				ImGui::ListBox("Object type", &t_currentItem, t_currentBrush->getObjectNames_C().data(), t_currentBrush->getObjectPropertiesLength());
				m_currentObjectProperty = t_currentBrush->m_objectProperties[t_currentItem];
			}

			if (ImGui::Button("Import Mesh")) {
				ifd::FileDialog::Instance().Open("TextureOpenDialog", "Open a File", "Mesh File (*.obj){.obj},.*");
			}

			if (ifd::FileDialog::Instance().IsDone("TextureOpenDialog")) {
				if (ifd::FileDialog::Instance().HasResult()) {
					const std::wstring& res = ifd::FileDialog::Instance().GetResult();
					std::string t_resultPath(res.begin(), res.end());

					int t_afterLastSlash = t_resultPath.find_last_of("\\") + 1;
					std::string t_restultObjectName = t_resultPath.substr(t_afterLastSlash, t_resultPath.size());

					t_currentBrush->addObjectType(new BiomeObjectProperty());
					t_currentBrush->getCurrentObjectType().m_name = t_restultObjectName;
					m_currentObjectProperty = &t_currentBrush->getCurrentObjectType();
				}
				ifd::FileDialog::Instance().Close();
			}
		}
	}

	ImGui::Separator();
	ImGui::Separator();
	ImGui::Separator();

	if (ImGui::Button("Create Biome")) {	 // Buttons return true when clicked (most widgets return true when edited/activated)
		m_objectManager.clearMeshes();
		Biome(m_objectManager, m_brushManager);
	}

	ImGui::Separator();
	ImGui::Separator();
	ImGui::Separator();

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