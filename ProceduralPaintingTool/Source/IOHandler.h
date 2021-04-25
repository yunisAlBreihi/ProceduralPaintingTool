#pragma once
#include <vector>
#include "BrushManager.h"
#include "ObjectManager.h"

namespace IOHandler {
	void saveJson_brush(const char* filename, BrushManager& brushManager);
	void loadJson_brush(const char* filename, BrushManager& brushManager);

	void saveJson_terrainVerticesColor(const char* filename, ObjectManager& objectManager);
	void loadJson_terrainVerticesColor(const char* filename, ObjectManager& objectManager, BrushManager& brushManager);

	void saveJson_attribute(const char* filename, const char* attributeName, int attribute);
	void loadJson_attribute(const char* filename, const char* attributeName, int& attribute);
}