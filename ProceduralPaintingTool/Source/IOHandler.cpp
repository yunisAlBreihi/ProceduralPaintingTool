#include "IOHandler.h"
#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using value_t = nlohmann::detail::value_t;

namespace IOHandler {
	void saveJson_brush(const char* filename, BrushManager& brushManager) {
		json t_json;
		const char* t_namePrefix = "Brush";

		const std::vector<BrushProperty*>& t_brushes = brushManager.getBrushes();
		int t_brushSize = t_brushes.size();
		for (size_t i = 0; i < t_brushSize; i++) {
			std::string t_brushName = t_namePrefix + std::to_string(i);
			std::string t_brushColorName = t_brushName + "_color";
			std::string t_brushCurrentObjectName = t_brushName + "_currentObject";
			std::string t_brushIdName = t_brushName + "_Id";

			int t_objectPropertiesSize = t_brushes[i]->m_objectProperties.size();

			t_json[t_brushName] = { {t_brushes[i]->m_vertexColor.r, t_brushes[i]->m_vertexColor.g, t_brushes[i]->m_vertexColor.b, t_brushes[i]->m_vertexColor.a },
									{ t_brushes[i]->m_current_object},
									{ t_brushes[i]->m_id} };

			for (size_t objectIndex = 0; objectIndex < t_objectPropertiesSize; ++objectIndex) {
				std::string t_objectPropertyName = t_brushName + "_object" + std::to_string(objectIndex);
				t_json[t_objectPropertyName] = { {t_brushes[i]->m_objectProperties[objectIndex]->m_sizeOffset.x,t_brushes[i]->m_objectProperties[objectIndex]->m_sizeOffset.y },
												 {t_brushes[i]->m_objectProperties[objectIndex]->m_positionOffsetX.x, t_brushes[i]->m_objectProperties[objectIndex]->m_positionOffsetX.y },
												 {t_brushes[i]->m_objectProperties[objectIndex]->m_positionOffsetY.x, t_brushes[i]->m_objectProperties[objectIndex]->m_positionOffsetY.y },
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
		if (t_i.fail()) {
			return;
		}

		t_i >> t_json;
		if (t_json == "null") {
			return;
		}

		std::vector<const char*> t_usedNames;
		for (auto it = t_json.begin(); it != t_json.end(); ++it) {
			t_usedNames.push_back(it.key().c_str());
		}

		BrushProperty* t_brushProperty = nullptr;
		BiomeObjectProperty* t_objectProperty = nullptr;
		std::string t_lastItemName;
		for (auto it = t_json.begin(); it != t_json.end(); ++it) {
			//std::cout << it.key().c_str() << std::endl;
			std::string t_itemName = it.key().c_str();

			int t_propertyIndex = 0;

			for (const auto& t_property : it.value()) {
				if (t_itemName.size() < t_lastItemName.size() || t_lastItemName == "") {
					switch (t_propertyIndex) {
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
					}
				}
				else {
					switch (t_propertyIndex) {
					case 0:
						t_objectProperty = new BiomeObjectProperty();
						t_objectProperty->m_sizeOffset = glm::vec2(t_property[0], t_property[1]);
						break;
					case 1:
						t_objectProperty->m_positionOffsetX = glm::vec2(t_property[0], t_property[1]);
						break;
					case 2:
						t_objectProperty->m_positionOffsetY = glm::vec2(t_property[0], t_property[1]);
						break;
					case 3:
						t_objectProperty->m_radius = t_property[0];
						break;
					case 4:
						t_objectProperty->m_frequency = t_property[0];
						break;
					case 5:
						t_objectProperty->m_priority = t_property[0];
						break;
					case 6:
						t_objectProperty->m_name = t_property[0];
						t_brushProperty->addObjectType(t_objectProperty);
						break;
					}
				}
				t_propertyIndex++;
			}
			if (t_itemName.size() < t_lastItemName.size() || t_lastItemName == "") {
				brushManager.addBrush(t_brushProperty);
			}
			else {
			}
			t_lastItemName = t_itemName;
		}
	}

	void save_attribute(const char* filename, const char* attributeName,int attribute) {
		json t_json;

		t_json[attributeName] = attribute;

		std::ofstream t_o(filename);
		t_o << t_json << std::endl;
	}

	void load_attribute(const char* filename, const char* attributeName, int& attribute) {
		json t_json;
		std::ifstream t_i(filename);
		if (t_i.fail()) {
			return;
		}

		t_i >> t_json;
		if (t_json == "null") {
			return;
		}

		attribute = t_json.find(attributeName).value();
	}
}