#pragma once
#include <vector>
#include "BrushProperty.h"
#include "Vertex.h"

class BrushManager {
private:
	std::vector<BrushProperty*> m_brushProperties;
	BrushProperty* m_currentBrushProperty = nullptr;

	int m_brushIdCount = -1;

public:
	BrushManager();

	void start();
	void quit();

	BrushProperty& createBrush(const char* name);
	void addBrush(BrushProperty* brushProperty);
	void deleteCurrentBrush();
	BrushProperty& setCurrentBrush(const int index);

	void removeVertexFromOtherBrushesAtPosition(const glm::vec3& position);
	void removeVerticesFromOtherBrushes();
	void removeVerticesFromAllBrushes();

	auto& getBrushes() { return m_brushProperties; }
	BrushProperty* getCurrentBrush() { return m_currentBrushProperty; }
	BrushProperty* getBrushWithColor(glm::vec4 color);
};