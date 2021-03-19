#include "Biome.h"

Biome::Biome(ObjectManager& objectManager, Brush brush) : m_objectManager(objectManager)
{
	std::vector<Mesh*> t_createdMeshes;
	float t_treeRadius = 4.0f;

	for (size_t x = 0; x < 10; ++x) {
		for (size_t z = 0; z < 10; ++z) {
			//float t_randomSize;
			//if (static_cast<int>(brush.m_sizeOffset.x * 100) != 0 || static_cast<int>(brush.m_sizeOffset.y * 100) != 0)
			//{
			//	const float t_randomSize = ((rand() % static_cast<int>(brush.m_sizeOffset.y * 100) + static_cast<int>(brush.m_sizeOffset.x * 100)) * 0.01f);
			//}
			const float t_randomSize = ((rand() % static_cast<int>(brush.m_sizeOffset.y * 100) + static_cast<int>(brush.m_sizeOffset.x * 100)) * 0.01f);
			const float t_offsetX = ((rand() % static_cast<int>(brush.m_positionOffsetX.y * 100) + static_cast<int>(brush.m_positionOffsetX.x * 100)) * 0.01f);
			const float t_offsetZ = ((rand() % static_cast<int>(brush.m_positionOffsetY.y * 100) + static_cast<int>(brush.m_positionOffsetY.x * 100)) * 0.01f);

			//const float t_offsetX = (rand() % 200 - 100) * 0.01f;
			//const float t_offsetZ = (rand() % 200 - 100) * 0.01f;
			const glm::vec3 t_treePos = glm::vec3(x * 3 + t_offsetX, 0.0f, z * 3 + t_offsetZ);

			bool t_invalidTree = false;
			for (const auto& t_createdMesh : t_createdMeshes)
			{
				if (t_treePos.x > t_createdMesh->getTransform().position.x - t_treeRadius &&
					t_treePos.x < t_createdMesh->getTransform().position.x + t_treeRadius &&
					t_treePos.z > t_createdMesh->getTransform().position.z - t_treeRadius &&
					t_treePos.z < t_createdMesh->getTransform().position.z + t_treeRadius)
				{
					t_invalidTree = true;
					break;
				}
			}
			if (t_invalidTree == true) {
				continue;
			}

			Mesh* t_mesh = new Mesh("Assets/Tree.obj", Transform());
			t_mesh->setPosition(t_treePos);
			t_mesh->setScale(glm::vec3(t_randomSize));
			t_createdMeshes.push_back(t_mesh);
		}
	}

	for (auto t_createdMesh : t_createdMeshes) {
		m_objectManager.m_meshes.push_back(t_createdMesh);
	}
}
