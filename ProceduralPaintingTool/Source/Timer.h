#pragma once
class Timer
{
private:
	float m_lastTime = 0.0f;

public:
	float deltaTime = 0.0f;
	float time = 0.0f;

public:
	Timer();

	void update();
};

