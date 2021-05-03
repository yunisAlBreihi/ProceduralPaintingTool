#pragma once
#include "glm/ext/vector_float3.hpp"
#include "Camera.h"
#include "Mesh.h"
#include "globals.h"
#include "MousePicker.h"

const Vertex& raycastFromCameraVertex(Camera& camera, MousePicker& mousePicker, Mesh& targetMesh, int traceSteps, float distance, float hitRadius) {
	const float stepDistance = distance / traceSteps;
	for (size_t i = 0; i < traceSteps; i++)	{
		glm::vec3 t_rayPosition = camera.getPosition() + mousePicker.getCurrentRay() * (stepDistance * i);
		Vertex t_targetVertex = targetMesh.getVertexAtPosition(t_rayPosition, hitRadius);
		if (t_targetVertex.position != glm::vec3(0.0f))	{
			globals::printVec3(t_targetVertex.position);
			return t_targetVertex;
		}
	}
	return Vertex{};
}

std::vector<Vertex*>& raycastFromCameraVertexRadius(Camera& camera, MousePicker& mousePicker, Mesh& targetMesh, int traceSteps, float distance, float hitRadius) {
	const float stepDistance = distance / traceSteps;
	for (size_t i = 0; i < traceSteps; i++)	{
		glm::vec3 t_rayPosition = camera.getPosition() + mousePicker.getCurrentRay() * (stepDistance * i);
		std::vector<Vertex*>& t_vertices = targetMesh.getVertexAtPositionRadius(t_rayPosition, hitRadius);
		if (t_vertices.empty() == false) {
			return t_vertices;
		}
	}
	std::vector<Vertex*> t_emptyVector;
	return t_emptyVector;
}