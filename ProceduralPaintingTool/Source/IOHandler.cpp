#include "IOHandler.h"


namespace IOHandler {
	void saveJson_brush(const char* filename, BrushManager& brushManager) {
		globals::g_hasBrushSave = true;

		json t_json;

		const std::vector<BrushProperty*>& t_brushes = brushManager.getBrushes();
		int t_brushSize = t_brushes.size();
		for (size_t i = 0; i < t_brushSize; i++) {
			std::string t_brushName = globals::g_brushPrefix + std::to_string(i);

			int t_objectPropertiesSize = t_brushes[i]->m_objectProperties.size();

			t_json[t_brushName] = { {t_brushes[i]->m_vertexColor.r, t_brushes[i]->m_vertexColor.g, t_brushes[i]->m_vertexColor.b, t_brushes[i]->m_vertexColor.a },
									{ t_brushes[i]->m_current_object},
									{ t_brushes[i]->m_id},
									{ t_brushes[i]->m_colorIsSet},
									{ t_brushes[i]->m_name} };

			for (size_t objectIndex = 0; objectIndex < t_objectPropertiesSize; ++objectIndex) {
				std::string t_objectPropertyName = t_brushName + globals::g_propertyPrefix + std::to_string(objectIndex);
				t_json[t_objectPropertyName] = { {t_brushes[i]->m_objectProperties[objectIndex]->m_sizeOffset.x,t_brushes[i]->m_objectProperties[objectIndex]->m_sizeOffset.y },
												 {t_brushes[i]->m_objectProperties[objectIndex]->m_positionOffsetX.x, t_brushes[i]->m_objectProperties[objectIndex]->m_positionOffsetX.y },
												 {t_brushes[i]->m_objectProperties[objectIndex]->m_positionOffsetY.x, t_brushes[i]->m_objectProperties[objectIndex]->m_positionOffsetY.y },
												 {t_brushes[i]->m_objectProperties[objectIndex]->m_rotationOffsetX.x, t_brushes[i]->m_objectProperties[objectIndex]->m_rotationOffsetX.y },
												 {t_brushes[i]->m_objectProperties[objectIndex]->m_rotationOffsetY.x, t_brushes[i]->m_objectProperties[objectIndex]->m_rotationOffsetY.y },
												 {t_brushes[i]->m_objectProperties[objectIndex]->m_rotationOffsetZ.x, t_brushes[i]->m_objectProperties[objectIndex]->m_rotationOffsetZ.y },
												 {t_brushes[i]->m_objectProperties[objectIndex]->m_radius },
												 {t_brushes[i]->m_objectProperties[objectIndex]->m_frequency },
												 {t_brushes[i]->m_objectProperties[objectIndex]->m_priority },
												 {t_brushes[i]->m_objectProperties[objectIndex]->m_name } };
			}
		}
		std::ofstream t_o(filename);
		t_o << t_json << std::endl;
	}

	void loadJson_brush(const char* filename, BrushManager& brushManager) {
		json t_json;
		std::ifstream t_i(filename);
		if (t_i.peek() == std::ifstream::traits_type::eof()) {
			return;
		}

		t_i >> t_json;
		if (t_json.empty() || t_json == "null") {
			return;
		}

		std::vector<const char*> t_usedNames;
		for (auto it = t_json.begin(); it != t_json.end(); ++it) {
			t_usedNames.push_back(it.key().c_str());
		}

		int t_brushIndex = -1;
		int t_propertyIndex = 0;
		BrushProperty* t_brushProperty = nullptr;
		BiomeObjectProperty* t_objectProperty = nullptr;
		for (auto it = t_json.begin(); it != t_json.end(); ++it) {
			std::string t_itemName = it.key().c_str();

			int t_itemIndex = 0;

			for (const auto& t_property : it.value()) {
				if (t_itemName._Starts_with(globals::g_brushPrefix + std::to_string(t_brushIndex) + globals::g_propertyPrefix) == false) {
					switch (t_itemIndex) {
					case 0:
						t_brushProperty = new BrushProperty();
						t_brushProperty->m_vertexColor = { t_property[0],t_property[1],t_property[2],t_property[3] };
						break;
					case 1:
						t_brushProperty->m_current_object = t_property[0];
						break;
					case 2:
						t_brushProperty->m_id = t_property[0];
						break;
					case 3:
						t_brushProperty->m_colorIsSet = t_property[0];
						break;
					case 4:
						t_brushProperty->m_name = t_property[0];
					}
				}
				else if (t_itemName._Starts_with(globals::g_brushPrefix + std::to_string(t_brushIndex) +globals::g_propertyPrefix) == true) {
					switch (t_itemIndex) {
					case 0:
						t_objectProperty = new BiomeObjectProperty();
						t_objectProperty->m_sizeOffset = { t_property[0], t_property[1] };
						break;
					case 1:
						t_objectProperty->m_positionOffsetX = { t_property[0], t_property[1] };
						break;
					case 2:
						t_objectProperty->m_positionOffsetY = { t_property[0], t_property[1] };
						break;
					case 3:
						t_objectProperty->m_rotationOffsetX = { t_property[0], t_property[1] };
						break;
					case 4:
						t_objectProperty->m_rotationOffsetY = { t_property[0], t_property[1] };
						break;
					case 5:
						t_objectProperty->m_rotationOffsetZ = { t_property[0], t_property[1] };
						break;
					case 6:
						t_objectProperty->m_radius = t_property[0];
						break;
					case 7:
						t_objectProperty->m_frequency = t_property[0];
						break;
					case 8:
						t_objectProperty->m_priority = t_property[0];
						break;
					case 9:
						t_objectProperty->m_name = t_property[0];
						t_brushProperty->addObjectType(t_objectProperty);
						t_propertyIndex++;
						break;
					}
				}
				t_itemIndex++;
			}
			if (t_itemName._Starts_with(globals::g_brushPrefix + std::to_string(t_brushIndex) + globals::g_propertyPrefix) == false) {
				brushManager.addBrush(t_brushProperty);
				t_brushIndex++;
			}
		}
	}

	void saveJson_terrainVerticesColor(const char* filename, ObjectManager& objectManager) {
		globals::g_hasTerrainVerticesSave = true;

		Vertex* t_vertices = objectManager.m_terrain->m_vertices;
		int t_vertex_count = objectManager.m_terrain->m_vert_count;
		json t_json;
		const char* t_namePrefix = "TerrainVertexColor_";

		glm::vec4 t_emptyColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		for (size_t i = 0; i < t_vertex_count; ++i) {
			if (t_vertices[i].color == t_emptyColor) {
				continue;
			}
			std::string t_vertexName = t_namePrefix + std::to_string(i);
			t_json[t_vertexName] = { {t_vertices[i].color.x, t_vertices[i].color.y, t_vertices[i].color.z, t_vertices[i].color.a},
									 {t_vertices[i].index} };
		}
		std::ofstream t_o(filename);
		t_o << t_json << std::endl;
	}

	void loadJson_terrainVerticesColor(const char* filename, ObjectManager& objectManager, BrushManager& brushManager) {
		json t_json;
		std::ifstream t_i(filename);
		if (t_i.peek() == std::ifstream::traits_type::eof()) {
			return;
		}

		t_i >> t_json;
		if (t_json.empty() || t_json == "null") {
			return;
		}
		Mesh& t_terrain = *objectManager.m_terrain;

		int t_parameterIndex = 0;
		bool t_nextUsable = false;
		glm::vec4 t_vertexColor;
		for (const auto& t_item : t_json) {
			for (const auto& t_parameter : t_item) {
				if (t_parameterIndex % 2 == 0) {
					if (t_parameter[0] != 0.0f || t_parameter[1] != 0.0f ||
						t_parameter[2] != 0.0f || t_parameter[3] != 1.0f) {
						t_vertexColor = { t_parameter[0] ,t_parameter[1], t_parameter[2], t_parameter[3] };
						t_nextUsable = true;
					}
				}
				else if (t_nextUsable == true) {
					t_terrain.setVertexColor(t_parameter[0], t_vertexColor);
					auto t_currentBrush = brushManager.getBrushWithColor(t_vertexColor);
					if (t_currentBrush != nullptr) {
						auto t_currentVertex = t_terrain.getVertexAtIndex(t_parameter[0]);
						t_currentBrush->addVertex(t_currentVertex);
					}
					t_nextUsable = false;
				}
				t_parameterIndex++;
			}
		}
	}
}