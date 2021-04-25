#pragma once
#include <vector>
#include "BrushManager.h"

namespace IOHandler {
	void saveJson_brush(const char* filename, BrushManager& brushManager);
	void loadJson_brush(const char* filename, BrushManager& brushManager);

	void save_attribute(const char* filename, const char* attributeName, int attribute);
	void load_attribute(const char* filename, const char* attributeName, int& attribute);
}