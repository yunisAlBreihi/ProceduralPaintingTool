#include "Timer.h"
#include "GLFW/glfw3.h"

Timer::Timer()
{
	m_lastTime = glfwGetTime();
}

void Timer::update()
{
	time = glfwGetTime();
	float deltaTime = time - m_lastTime;
	m_lastTime = time;
}
