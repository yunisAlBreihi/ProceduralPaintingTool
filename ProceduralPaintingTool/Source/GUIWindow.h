#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Transform.h"

class GUIWindow
{
private:

	int m_counter = 0;
	Transform& m_transform;

public:
	GUIWindow(Transform& transform) : m_transform(transform) {};
	void update();
	void render(GLFWwindow* window);

	void updateSliders(Transform& transform);
};