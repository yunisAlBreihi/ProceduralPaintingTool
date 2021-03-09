#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Transform.h"
#include "ObjectManager.h"

class GUIWindow
{
private:

	ObjectManager& m_objectManager;
	int m_counter = 0;
	Transform m_transform;

public:
	GUIWindow(Transform transform, ObjectManager& objectManager) : m_transform(transform), m_objectManager(objectManager) {};
	void update();
	void render(GLFWwindow* window);
	void updateSliders(Transform transform);
};