#pragma once
#include "glm/ext/vector_float3.hpp"
#include "Camera.h"
#include "Mesh.h"


const glm::vec3& traceFromCamera(Camera& camera, Mesh& targetMesh, const int& traceSteps, const float& stepDistance, const float& hitRadius ) 
{
	for (size_t i = 0; i < traceSteps; i++)
	{
		glm::vec3 t_targetLocation = camera.getPosition() + (camera.forward() * (i * stepDistance));
	}
}