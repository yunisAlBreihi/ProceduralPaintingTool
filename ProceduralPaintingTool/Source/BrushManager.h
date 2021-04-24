#pragma once
#include <vector>
#include "BrushProperty.h"
#include "Vertex.h"

class BrushManager {
private:
	std::vector<BrushProperty*> m_brushProperties;
	BrushProperty* m_currentBrushProperty;

	int m_brushIdCount = -1;

public:
	void start();
	void quit();

	BrushProperty& createBrush();
	void addBrush(const BrushProperty& brushProperty);
	BrushProperty& setCurrentBrush(const int index);

	void removeVertexFromOtherBrushesAtPosition(const glm::vec3& position);
	void removeVerticesFromOtherBrushes();

	auto& getBrushes() { return m_brushProperties; }
	BrushProperty* getCurrentBrush() { return m_currentBrushProperty; }
};