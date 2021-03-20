#include "Biome.h"

Biome::Biome(ObjectManager& objectManager, Brush brush) : m_objectManager(objectManager)
{
	std::vector<Mesh*> t_createdMeshes;

	float t_terainRadius = 1.0f;

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
			glm::vec3 t_treePos = glm::vec3(x * 3 + t_offsetX, 0.0f, z * 3 + t_offsetZ);

			bool t_invalidTree = false;
			for (const auto& t_createdMesh : t_createdMeshes)
			{
				if (t_treePos.x > t_createdMesh->getTransform().position.x - brush.m_treeRadius &&
					t_treePos.x < t_createdMesh->getTransform().position.x + brush.m_treeRadius &&
					t_treePos.z > t_createdMesh->getTransform().position.z - brush.m_treeRadius &&
					t_treePos.z < t_createdMesh->getTransform().position.z + brush.m_treeRadius)
				{
					t_invalidTree = true;
					break;
				}
			}
			if (t_invalidTree == true) {
				continue;
			}

			//Check if tree is near a terrain vertex
			for (size_t i = 0; i < m_objectManager.m_terrain->m_vert_count; i++)
			{
				if (t_treePos.x > m_objectManager.m_terrain->m_verts[i].position.x - t_terainRadius &&
					t_treePos.x < m_objectManager.m_terrain->m_verts[i].position.x + t_terainRadius &&
					t_treePos.z > m_objectManager.m_terrain->m_verts[i].position.z - t_terainRadius &&
					t_treePos.z < m_objectManager.m_terrain->m_verts[i].position.z + t_terainRadius)
				{
					t_treePos.y = m_objectManager.m_terrain->m_verts[i].position.y;
					break;
				}
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
