#pragma once
#include <vector>
#include "BrushProperty.h"
#include "Vertex.h"

class BrushManager {
private:
	std::vector<BrushProperty*> m_brushProperties;
	BrushProperty* m_currentBrushProperty;

public:

	BrushProperty& CreateBrush();

	BrushProperty& setCurrentBrush(int index);

	auto& getBrushes() { return m_brushProperties; }
	BrushProperty* getCurrentBrush() { return m_currentBrushProperty; }

};