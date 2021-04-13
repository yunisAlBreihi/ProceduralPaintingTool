#pragma once
#include "Mesh.h"

struct BiomeBrush
{
public:
	Mesh* m_mesh = nullptr;
	float m_radius = 0;
	int m_priority = 0;

	BiomeBrush(Mesh* mesh, float radius, int priority) : m_mesh(mesh), m_radius(radius), m_priority(priority) {};
};