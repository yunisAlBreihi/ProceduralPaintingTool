#pragma once
#include <vector>
#include <string>
#include "BiomeObjectProperty.h"

struct BrushProperty {
public:
	std::vector<BiomeObjectProperty*> m_objectProperties;
	int m_current_object = -1;
	float m_vertexColor[4] = { 1.0f,0.0f,1.0f,1.0f };

	std::vector<const char*> getObjectNames_C() {
		std::vector<const char*> t_objectTypes;

		for (const auto& t_objectType : m_objectProperties) {
			t_objectTypes.push_back(t_objectType->m_name.c_str());
		}
		return t_objectTypes;
	}

	BiomeObjectProperty& getCurrentObjectType() {
		return *m_objectProperties[m_current_object];
	}

	void addObjectType(BiomeObjectProperty* objectProperty) {
		m_objectProperties.push_back(objectProperty);
		m_current_object = m_objectProperties.size() - 1;
	}

	const int getObjectPropertiesLength() {
		return m_objectProperties.size();
	}
};