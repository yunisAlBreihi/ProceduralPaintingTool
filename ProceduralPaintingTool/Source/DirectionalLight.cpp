#include "DirectionalLight.h"

void DirectionalLight::update(Shader* shader, const glm::vec3& direction,const glm::vec3& cameraPosition)
{
	shader->setVec3("u_DirectionalLight", direction);
	shader->setVec3("u_EyePosition", cameraPosition);
}
