#pragma once
#include "ObjectManager.h"
#include "BrushManager.h"

class Biome {
private:
	ObjectManager& m_objectManager;
	BrushManager& m_brushManager;

	void createBiome();

public:
	Biome(ObjectManager& objectManager, BrushManager& brushManager);
};