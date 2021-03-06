#pragma once
#include <vector>
#include <string>
#include "BiomeObjectProperty.h"
#include "Vertex.h"

struct BrushProperty {
public:
	glm::vec4 m_vertexColor = { 1.0f,0.0f,1.0f,1.0f };
	int m_current_object = -1;
	int m_id = -1;
	bool m_colorIsSet = false;
	std::string m_name = "";

	std::vector<BiomeObjectProperty*> m_objectProperties;
	std::vector<Vertex*> m_usedVertices;

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
		m_current_object = static_cast<int>(m_objectProperties.size() - 1);
	}

	void setCurrentObjectType(unsigned int index) {
		if (index < m_objectProperties.size()) {
			m_current_object = index;
		}
	}

	void deleteCurrentObjectType() {
		m_objectProperties[m_current_object] = nullptr;
		m_current_object -= 1;

		//remove the nullptr that was created
		auto t_iterator_to_remove = m_objectProperties.begin();
		for (auto it = m_objectProperties.begin(); it != m_objectProperties.end(); ++it) {
			if (*it == nullptr) {
				t_iterator_to_remove = it;
			}
		}
		m_objectProperties.erase(t_iterator_to_remove);
	}

	int getObjectPropertiesLength() {
		return static_cast<int>(m_objectProperties.size());
	}

	void addVertex(Vertex* vertex) {
		for (const auto& t_vertex : m_usedVertices) {
			if (t_vertex != nullptr) {
				if (vertex->position == t_vertex->position || vertex->color != m_vertexColor) {
					return;
				}
			}
		}
		m_usedVertices.push_back(vertex);
	}

	void removeVertexAtPosition(glm::vec3 position) {
		int t_arraySize = static_cast<int>(m_usedVertices.size());
		for (int i = 0; i < t_arraySize; i++) {
			if (m_usedVertices[i] != nullptr) {
				if (m_usedVertices[i]->position == position) {
					m_usedVertices[i] = nullptr;
				}
			}
		}
	}

	void removeAllVertices() {
		int t_arraySize = static_cast<int>(m_usedVertices.size());
		for (int i = 0; i < t_arraySize; i++) {
			if (m_usedVertices[i] != nullptr) {
				m_usedVertices[i] = nullptr;
			}
		}
	}
};