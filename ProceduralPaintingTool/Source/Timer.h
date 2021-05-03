#pragma once
class Timer {
private:
	float m_lastTime = 0.0f;

public:
	float m_deltaTime = 0.0f;
	float m_time = 0.0f;

public:
	Timer();

	void update();
};

