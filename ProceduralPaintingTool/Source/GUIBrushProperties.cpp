#include "GUIBrushProperties.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Biome.h"
#include "imgui/ImFileDialog.h"

void GUIBrushProperties::showBrushesButtons(const std::vector<BrushProperty*>& brushes, int& item) {
	for (int i = 0; i < brushes.size(); ++i) {
		if (ImGui::Button(brushes[i]->m_name.c_str())) {
			m_brushManager.setCurrentBrush(i);
			item = 0;
			//gets the current object object type, since the editor will crash if the brush that is changed to is shorter than the last.
			if (m_currentBrush != nullptr && m_currentBrush->getObjectPropertiesLength() != 0) {
				m_currentObjectProperty = &m_currentBrush->getCurrentObjectType();
			}
			else if (m_currentBrush != nullptr) {
				m_currentBrush->m_current_object = -1;
			}
		}
		ImGui::SameLine();
	}
}

void GUIBrushProperties::collapsingPropertyMenu(int& item) {
	if (ImGui::CollapsingHeader("Biome Properties")) {
		if (m_currentBrush->m_current_object != -1) {
			ImGui::SliderFloat2("Size Range", glm::value_ptr(m_currentObjectProperty->m_sizeOffset), 0.1f, 10.0f);
			ImGui::SliderFloat2("Position Range X", glm::value_ptr(m_currentObjectProperty->m_positionOffsetX), -10.0f, 10.0f);
			ImGui::SliderFloat2("Position Range Y", glm::value_ptr(m_currentObjectProperty->m_positionOffsetY), -10.0f, 10.0f);
			ImGui::SliderFloat2("Rotation Range X", glm::value_ptr(m_currentObjectProperty->m_rotationOffsetX), -360.0f, 360.0f);
			ImGui::SliderFloat2("Rotation Range Y", glm::value_ptr(m_currentObjectProperty->m_rotationOffsetY), -360.0f, 360.0f);
			ImGui::SliderFloat2("Rotation Range Z", glm::value_ptr(m_currentObjectProperty->m_rotationOffsetZ), -360.0f, 360.0f);
			ImGui::SliderFloat("Tree Radius", &m_currentObjectProperty->m_radius, 0.1f, 10.0f);
			ImGui::SliderInt("Frequency", &m_currentObjectProperty->m_frequency, 0, 20);
			ImGui::SliderInt("Priority", &m_currentObjectProperty->m_priority, 0, 20);
		}

		//listbox for current object properties on the brush
		if (m_currentBrush->getObjectPropertiesLength() != 0) {
			ImGui::ListBox("Object type", &item, m_currentBrush->getObjectNames_C().data(), m_currentBrush->getObjectPropertiesLength());
			m_currentBrush->setCurrentObjectType(item);
			m_currentObjectProperty = &m_currentBrush->getCurrentObjectType();
		}

		//Import mesh button. This calls for a file selector window to be able to open up.
		if (ImGui::Button("Import Mesh")) {
			ifd::FileDialog::Instance().Open("TextureOpenDialog", "Open a File", "Mesh File (*.obj){.obj},.*");
		}

		//This will display the file selection window
		if (ifd::FileDialog::Instance().IsDone("TextureOpenDialog")) {
			//When selecting an object, get the object name only, which is used to get a mesh in the Biome class.
			if (ifd::FileDialog::Instance().HasResult()) {
				const std::wstring& res = ifd::FileDialog::Instance().GetResult();
				std::string t_resultPath(res.begin(), res.end());

				int t_afterLastSlash = t_resultPath.find_last_of("\\") + 1;
				std::string t_restultObjectName = t_resultPath.substr(t_afterLastSlash, t_resultPath.size());

				m_currentBrush->addObjectType(new BiomeObjectProperty());
				m_currentBrush->getCurrentObjectType().m_name = t_restultObjectName;
				m_currentObjectProperty = &m_currentBrush->getCurrentObjectType();
			}
			ifd::FileDialog::Instance().Close();
		}

		ImGui::SameLine();
		if (ImGui::Button("Delete Current Mesh")) {
			m_currentBrush->deleteCurrentObjectType();
		}
	}
}

void GUIBrushProperties::separatorCreator(int count) {
	for (size_t i = 0; i < count; ++i) {
		ImGui::Separator();
	}
}

GUIBrushProperties::GUIBrushProperties(ObjectManager& objectManager, BrushManager& brushManager) :
	m_objectManager(objectManager), m_brushManager(brushManager) { }

void GUIBrushProperties::start() {

	//This lambda is used to get the file selection window to work. It's bound in the ImFileDialog
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

	//Int field for setting the randomizer seed.
	static int t_seed = 0;
	ImGui::InputInt("Seed", &globals::g_seed);
	ImGui::SliderFloat("Brush Radius", &globals::g_brush_radius, 1.0f, 4.0f);

	ImGui::Separator();

	auto& t_brushes = m_brushManager.getBrushes();

	//set the current brush and the current object property, so they are not empty when called later, if there is any
	m_currentBrush = m_brushManager.getCurrentBrush();
	if (m_currentBrush != nullptr && m_currentBrush->m_current_object != -1 && m_currentObjectProperty == nullptr) {
		m_currentObjectProperty = &m_currentBrush->getCurrentObjectType();
	}

	//Creates a button for each brush that currently exist
	static int t_currentItem = 0;
	showBrushesButtons(t_brushes, t_currentItem);

	ImGui::Separator();

	//Create brush, by opening a popup with a field to specify the brush name
	if (ImGui::Button("Create Brush")) {
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

	ImGui::SameLine();
	//display a delete brush button, only if its not nullptr of the eraser.
	if (m_currentBrush != nullptr && m_currentBrush->m_id != 0) {
		if (ImGui::Button("Delete Current Brush")) {
			m_brushManager.deleteCurrentBrush();
		}
	}

	//show the brush color only if the brush hasn't been used in a biome already, otherwise it will be hidden
	//also hidden if the current brush si the eraser.
	if (m_currentBrush != nullptr && m_currentBrush->m_id != 0 && m_currentBrush->m_colorIsSet == false) {
		if (ImGui::CollapsingHeader("Brush Color")) {
			ImGui::ColorPicker4("Brush Vertex Color", (float*)&m_currentBrush->m_vertexColor);
		}
	}

	//shows menu for object properties if it is allowed, otherwise stay hidden.
	//also hidden if the current brush is the eraser.
	if (m_currentBrush != nullptr && m_currentBrush->m_id != 0) {
		//button to fill the terrain with one color
		if (ImGui::Button("Fill terrain with color")) {
			m_objectManager.fillTerrainColor(m_currentBrush->m_vertexColor);
		}

		ImGui::Separator();

		//Collapsing menu for the object properties
		collapsingPropertyMenu(t_currentItem);
	}

	separatorCreator(3);

	//Button to create a biome.
	if (ImGui::Button("Create Biome")) {
		m_objectManager.clearMeshes();
		Biome(m_objectManager, m_brushManager);
	}

	separatorCreator(3);

	//Button to remove the meshes that are displayed at the moment
	if (ImGui::Button("Clear Meshes")) {
		m_objectManager.clearMeshes();
	}

	ImGui::SameLine();
	//Button to remove the meshes that are displayed at the moment
	if (ImGui::Button("Remove Terrain Colors")) {
		m_objectManager.fillTerrainColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
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