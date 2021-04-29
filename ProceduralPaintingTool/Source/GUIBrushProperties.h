#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Transform.h"
#include "ObjectManager.h"
#include "BrushProperty.h"
#include "BrushManager.h"

class GUIBrushProperties
{
private:

	ObjectManager& m_objectManager;
	BrushManager& m_brushManager;
	Transform m_transform;

	BiomeObjectProperty* m_currentObjectProperty;

	int m_brushCount = 0;
	int m_currentBrushIndex = 0;

	static const int m_bufferSize = 512;
	char m_buffer[m_bufferSize] = "Assets/";

public:
	GUIBrushProperties(ObjectManager& objectManager, BrushManager& brushManager);
	void start();
	void update();
	void render(GLFWwindow* window);
};