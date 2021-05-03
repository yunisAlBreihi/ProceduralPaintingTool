#include "GUIBrushProperties.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Biome.h"
#include "imgui/ImFileDialog.h"

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
	ImGui::Begin(m_GUIName);

	//Int field for setting the randomizer seed.
	static int t_seed = 0;
	ImGui::InputInt(m_seedName, &globals::g_seed);
	ImGui::SliderFloat(m_brushRadiusName, &globals::g_brush_radius, m_brushRadiusMin, m_brushRadiusMax);

	ImGui::Separator();

	auto& t_brushes = m_brushManager.getBrushes();

	//set the current brush and the current object property, so they are not empty when called later, if there is any
	m_currentBrush = m_brushManager.getCurrentBrush();
	if (m_currentBrush != nullptr && m_currentBrush->m_current_object != -1 && m_currentObjectProperty == nullptr) {
		m_currentObjectProperty = &m_currentBrush->getCurrentObjectType();
	}

	//Creates a button for each brush that currently exist
	static int t_currentItem = 0;
	showBrushButtons(t_brushes, t_currentItem);

	separatorCreator(2);

	//Create brush, by opening a popup with a field to specify the brush name
	if (ImGui::Button(m_createBrushName)) {
		ImGui::OpenPopup(m_brushNamingPopupName);
	}
	if (ImGui::BeginPopup(m_brushNamingPopupName)) {
		if (ImGui::InputText(m_createBrushInputName, m_buffer, m_bufferSize, ImGuiInputTextFlags_EnterReturnsTrue)) {
			ImGui::CloseCurrentPopup();
			m_brushManager.createBrush(m_buffer);
		}
		ImGui::EndPopup();
	}

	ImGui::SameLine();
	//display a delete brush button, only if its not nullptr of the eraser.
	if (m_currentBrush != nullptr && m_currentBrush->m_id != 0) {
		if (ImGui::Button(m_deleteBrushName)) {
			m_brushManager.deleteCurrentBrush();
		}
	}

	//show the brush color only if the brush hasn't been used in a biome already, otherwise it will be hidden
	//also hidden if the current brush si the eraser.
	if (m_currentBrush != nullptr && m_currentBrush->m_id != 0 && m_currentBrush->m_colorIsSet == false) {
		if (ImGui::CollapsingHeader(m_headerBrushColorName)) {
			ImGui::ColorPicker4(m_colorPickerName, (float*)&m_currentBrush->m_vertexColor);
		}
	}

	//shows menu for object properties if it is allowed, otherwise stay hidden.
	//also hidden if the current brush is the eraser.
	if (m_currentBrush != nullptr && m_currentBrush->m_id != 0) {
		//button to fill the terrain with one color
		if (ImGui::Button(m_fillTerrainColorName)) {
			m_objectManager.fillTerrainColor(m_currentBrush->m_vertexColor);
		}

		ImGui::Separator();

		//Collapsing menu for the object properties
		collapsingPropertyMenu(t_currentItem);
	}

	separatorCreator(3);

	//Button to create a biome.
	if (ImGui::Button(m_createBiomeName)) {
		m_objectManager.clearMeshes();
		Biome(m_objectManager, m_brushManager);
	}

	separatorCreator(3);

	//Button to remove the meshes that are displayed at the moment
	if (ImGui::Button(m_clearMeshesName)) {
		m_objectManager.clearMeshes();
	}

	ImGui::SameLine();
	//Button to remove the meshes that are displayed at the moment
	if (ImGui::Button(m_RemoveTerrainColorsName)) {
		m_objectManager.fillTerrainColor(globals::EMPTY_COLOR_V);
	}
	ImGui::End();
}

void GUIBrushProperties::render(GLFWwindow* window) {
	ImGuiIO& t_io = ImGui::GetIO();
	int t_width, t_height;
	glfwGetWindowSize(window, &t_width, &t_height);
	t_io.DisplaySize = ImVec2(static_cast<float>(t_width), static_cast<float>(t_height));

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (t_io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		GLFWwindow* t_backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(t_backup_current_context);
	}
}

void GUIBrushProperties::showBrushButtons(const std::vector<BrushProperty*>& brushes, int& item) {
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
	if (ImGui::CollapsingHeader(m_headerPriopertiesName)) {
		if (m_currentBrush->m_current_object != -1) {
			ImGui::SliderFloat2(m_sliderSizeRangeName, glm::value_ptr(m_currentObjectProperty->m_sizeOffset), m_sizeRangeMin, m_sizeRangeMax);
			ImGui::SliderFloat2(m_sliderPosRangeXName, glm::value_ptr(m_currentObjectProperty->m_positionOffsetX), m_posRangeXMin, m_posRangeXMax);
			ImGui::SliderFloat2(m_sliderPosRangeYName, glm::value_ptr(m_currentObjectProperty->m_positionOffsetY), m_posRangeYMin, m_posRangeYMax);
			ImGui::SliderFloat2(m_sliderRotRangeXName, glm::value_ptr(m_currentObjectProperty->m_rotationOffsetX), m_rotRangeXMin, m_rotRangeXMax);
			ImGui::SliderFloat2(m_sliderRotRangeYName, glm::value_ptr(m_currentObjectProperty->m_rotationOffsetY), m_rotRangeYMin, m_rotRangeYMax);
			ImGui::SliderFloat2(m_sliderRotRangeZName, glm::value_ptr(m_currentObjectProperty->m_rotationOffsetZ), m_rotRangeZMin, m_rotRangeZMax);
			ImGui::SliderFloat(m_objectRadiusName, &m_currentObjectProperty->m_radius, m_objectRadiusMin, m_objectRadiusMax);
			ImGui::SliderInt(m_frequencyName, &m_currentObjectProperty->m_frequency, m_frequencyMin, m_frequencyMax);
			ImGui::SliderInt(m_priorityName, &m_currentObjectProperty->m_priority, m_priorityMin, m_priorityMax);
		}

		//listbox for current object properties on the brush
		if (m_currentBrush->getObjectPropertiesLength() != 0) {
			ImGui::ListBox(m_objectListName, &item, m_currentBrush->getObjectNames_C().data(), m_currentBrush->getObjectPropertiesLength());
			m_currentBrush->setCurrentObjectType(item);
			m_currentObjectProperty = &m_currentBrush->getCurrentObjectType();
		}

		//Import mesh button. This calls for a file selector window to be able to open up.
		if (ImGui::Button(m_importMeshName)) {
			ifd::FileDialog::Instance().Open(m_dialogueTitleName, m_dialogueName, m_dialogueTypes);
		}

		//This will display the file selection window
		if (ifd::FileDialog::Instance().IsDone(m_dialogueTitleName)) {
			//When selecting an object, get the object name only, which is used to get a mesh in the Biome class.
			if (ifd::FileDialog::Instance().HasResult()) {
				const std::wstring& res = ifd::FileDialog::Instance().GetResult();
				std::string t_resultPath(res.begin(), res.end());

				int t_afterLastSlash = static_cast<int>(t_resultPath.find_last_of("\\") + 1);
				std::string t_restultObjectName = t_resultPath.substr(t_afterLastSlash, t_resultPath.size());

				m_currentBrush->addObjectType(new BiomeObjectProperty());
				m_currentBrush->getCurrentObjectType().m_name = t_restultObjectName;
				m_currentObjectProperty = &m_currentBrush->getCurrentObjectType();
			}
			ifd::FileDialog::Instance().Close();
		}

		ImGui::SameLine();
		if (ImGui::Button(m_deleteCurrentMeshName)) {
			m_currentBrush->deleteCurrentObjectType();
		}
	}
}

void GUIBrushProperties::separatorCreator(int count) {
	for (size_t i = 0; i < count; ++i) {
		ImGui::Separator();
	}
}