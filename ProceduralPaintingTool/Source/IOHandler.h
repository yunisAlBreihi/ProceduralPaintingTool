#pragma once
#include <vector>
#include "BrushManager.h"

void saveJson_brush(const char* filename, BrushManager& brushManager);
void loadJson_brush(const char* filename, BrushManager& brushManager);