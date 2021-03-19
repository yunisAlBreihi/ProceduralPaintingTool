#pragma once
#include "ObjectManager.h"

class Biome 
{
private:
	ObjectManager& m_objectManager;

public:
	Biome(ObjectManager& objectManager, Brush brush);
};