#include "IOHandler.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>

using json = nlohmann::json;
using value_t = nlohmann::detail::value_t;

void saveJson_brush(const char* filename, const std::vector<BrushProperty*>& brushes) {
	json t_json;
	const char* t_namePrefix = "Brush";

	int t_brushSize = brushes.size();
	for (size_t i = 0; i < t_brushSize; i++) {
		std::string t_brushName = t_namePrefix + std::to_string(i);
		std::string t_brushColorName = t_brushName + "_color";
		std::string t_brushCurrentObjectName = t_brushName + "_currentObject";
		std::string t_brushIdName = t_brushName + "_Id";

		int t_objectPropertiesSize = brushes[i]->m_objectProperties.size();

		t_json[t_brushName] = { {brushes[i]->m_vertexColor.r, brushes[i]->m_vertexColor.g, brushes[i]->m_vertexColor.b, brushes[i]->m_vertexColor.a },
								{ brushes[i]->m_current_object},
								{ brushes[i]->m_id} };

		for (size_t objectIndex = 0; objectIndex < t_objectPropertiesSize; ++objectIndex) {
			std::string t_objectPropertyName = t_brushName + "_object" + std::to_string(objectIndex);
			t_json[t_objectPropertyName] = { {brushes[i]->m_objectProperties[objectIndex]->m_sizeOffset.x,brushes[i]->m_objectProperties[objectIndex]->m_sizeOffset.y },
											 {brushes[i]->m_objectProperties[objectIndex]->m_positionOffsetX.x, brushes[i]->m_objectProperties[objectIndex]->m_positionOffsetX.y },
											 {brushes[i]->m_objectProperties[objectIndex]->m_positionOffsetY.x, brushes[i]->m_objectProperties[objectIndex]->m_positionOffsetY.y },
											 {brushes[i]->m_objectProperties[objectIndex]->m_radius },
											 {brushes[i]->m_objectProperties[objectIndex]->m_frequency },
											 {brushes[i]->m_objectProperties[objectIndex]->m_priority },
											 {brushes[i]->m_objectProperties[objectIndex]->m_name } };
		}
	}
	std::ofstream t_o(filename);
	t_o << t_json << std::endl;
}

void loadJson_brush(const char* filename, const std::vector<BrushProperty*>& brushes) {
	json t_json;
	std::ifstream t_i(filename);
	t_i >> t_json;

	BrushProperty t_brushProperty;
	BiomeObjectProperty t_objectProperty;


	int propertyIndex = 0;
	for (const auto& t_item : t_json) {
		std::cout << t_item << std::endl;

		for (const auto& t_property : t_item) {
			std::cout << t_property << std::endl;

			if (propertyIndex == 0) {
				t_brushProperty.m_vertexColor = glm::vec4(t_property[0], t_property[1], t_property[2], t_property[3]);
			}
			else if (propertyIndex == 1 || propertyIndex == 2 || propertyIndex == 6 || propertyIndex == 7 || propertyIndex == 8 || propertyIndex == 9) {
				t_brushProperty.m_current_object = t_property[0];
			}
			else if (propertyIndex == 3 || propertyIndex == 4 || propertyIndex == 5) {
				t_objectProperty.m_sizeOffset = glm::vec2(t_property[0], t_property[1]);
			}
			propertyIndex++;
		}

	}
}