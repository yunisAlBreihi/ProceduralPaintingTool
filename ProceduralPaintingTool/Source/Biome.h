#pragma once
#include "ObjectManager.h"
#include "BrushManager.h"

class Biome 
{
private:
	ObjectManager& m_objectManager;
	BrushManager& m_brushManager;

public:
	Biome(ObjectManager& objectManager, BrushManager& brushManager);
};