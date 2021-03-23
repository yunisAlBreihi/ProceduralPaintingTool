#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Transform.h"
#include "ObjectManager.h"
#include "Brush.h"

class GUIWindow
{
private:

	ObjectManager& m_objectManager;
	int m_counter = 0;
	Transform m_transform;

	std::vector<Brush*> m_brushes;
	Brush* m_currentBrush;

public:
	GUIWindow(Transform transform, ObjectManager& objectManager);
	void update();
	void render(GLFWwindow* window);
	void updateSliders(Transform transform);
};