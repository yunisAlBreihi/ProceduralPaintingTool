#pragma once
#include "glm/ext/vector_float3.hpp"
#include "Camera.h"
#include "Mesh.h"
#include "globals.h"
#include "MousePicker.h"

const glm::vec3& raycastFromCamera(Camera& camera, MousePicker& mousePicker, Mesh& targetMesh, int traceSteps, float distance, float hitRadius)
{
	const float stepDistance = distance / traceSteps;
	for (size_t i = 0; i < traceSteps; i++)
	{
		glm::vec3 t_rayPosition = camera.getPosition() + mousePicker.getCurrentRay() * (stepDistance * i);
		glm::vec3 t_targetPosition = targetMesh.getVertexAtPosition(t_rayPosition, hitRadius);
		if (t_targetPosition != glm::vec3(0.0f))
		{
			globals::printVec3(t_targetPosition);
			return t_targetPosition;
		}
	}
	return glm::vec3();
}