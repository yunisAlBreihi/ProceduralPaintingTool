#include "IOHandler.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>

using json = nlohmann::json;
using value_t = nlohmann::detail::value_t;

void saveJson_brush(const char* filename,const std::vector<BrushProperty*>& brushes) {
	json t_json;
	const char* t_namePrefix = "Brush";

	int t_brushSize = brushes.size();
	for (size_t i = 0; i < t_brushSize; i++) {
		std::string t_brushName = t_namePrefix + std::to_string(i);

		int t_objectPropertiesSize = brushes[i]->m_objectProperties.size();
		for (size_t objectIndex = 0; objectIndex < t_objectPropertiesSize; ++objectIndex) {
			t_json[t_brushName] = { {brushes[i]->m_objectProperties[objectIndex]->m_sizeOffset.x,brushes[i]->m_objectProperties[objectIndex]->m_sizeOffset.y },
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

void loadJson_brush(const char* filename,const std::vector<BrushProperty*>& brushes) {
	json t_json;
	std::ifstream t_i("brushes.json");
	t_i >> t_json;

	for (const auto& t_item : t_json) {
		std::cout << t_json << std::endl;
	}
}