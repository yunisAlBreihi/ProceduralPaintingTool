#include "Timer.h"
#include "GLFW/glfw3.h"

Timer::Timer() {
	m_lastTime = static_cast<float>(glfwGetTime());
}

void Timer::update() {
	m_time = static_cast<float>(glfwGetTime());
	m_deltaTime = m_time - m_lastTime;
	m_lastTime = m_time;
}
