#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Transform.h"
#include "ObjectManager.h"
#include "BiomeProperty.h"

class GUIBrushProperties
{
private:

	ObjectManager& m_objectManager;
	Transform m_transform;

	std::vector<BiomeProperty*> m_biomeProperties;
	BiomeProperty* m_currentBiomeProperty;
	BiomeObjectProperty* m_currentObjectProperty;

	int m_brushCount = 0;
	int m_currentBrushIndex = 0;

	static const int m_bufferSize = 512;
	char m_buffer[m_bufferSize] = "Assets/";

public:
	GUIBrushProperties(ObjectManager& objectManager);
	void update();
	void render(GLFWwindow* window);
};