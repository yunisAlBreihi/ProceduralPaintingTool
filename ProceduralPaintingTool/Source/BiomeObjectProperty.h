#pragma once
#include <glm/glm.hpp>
#include <string>


struct BiomeObjectProperty
{
public:
	glm::vec2 m_sizeOffset{ 0.9f, 1.1f };
	glm::vec2 m_positionOffsetX{ -3.0f, 3.0f };
	glm::vec2 m_positionOffsetY{ -3.0f, 3.0f };
	glm::vec2 m_rotationOffsetX{ -10.0f, 10.0f };
	glm::vec2 m_rotationOffsetY{ -10.0f, 10.0f };
	glm::vec2 m_rotationOffsetZ{ -10.0f, 10.0f };
	float m_radius{ 2.0f };
	int m_frequency{ 1 };
	int m_priority{ 0 };
	std::string m_name = "";
};