#pragma once
#include <vector>
#include "BrushProperty.h"

void saveJson_brush(const char* filename, const std::vector<BrushProperty*>& brushes);
void loadJson_brush(const char* filename, const std::vector<BrushProperty*>& brushes);