#include "Biome.h"
#include "BiomeObject.h"

Biome::Biome(ObjectManager& objectManager, const std::vector<Brush*>& brushes) : m_objectManager(objectManager)
{
	std::vector<BiomeObject*> t_createdObjects;
	float t_terainRadius = 1.0f;
	const char* object_path = "Assets/Tree.obj";

	int t_objectID = 0;

	for (const auto& t_brush : brushes)
	{
		t_objectID++;
		for (size_t x = 0; x < 10; ++x) {
			for (size_t z = 0; z < 10; ++z) {
				for (size_t i = 0; i < t_brush->m_frequency; ++i)
				{
					const float t_randomSize = ((rand() % static_cast<int>(t_brush->m_sizeOffset.y * 100) + static_cast<int>(t_brush->m_sizeOffset.x * 100)) * 0.01f);
					const float t_offsetX = ((rand() % static_cast<int>(t_brush->m_positionOffsetX.y * 100) + static_cast<int>(t_brush->m_positionOffsetX.x * 100)) * 0.01f);
					const float t_offsetZ = ((rand() % static_cast<int>(t_brush->m_positionOffsetY.y * 100) + static_cast<int>(t_brush->m_positionOffsetY.x * 100)) * 0.01f);

					glm::vec3 t_treePos = glm::vec3(x * 3 + t_offsetX, 0.0f, z * 3 + t_offsetZ);

					bool t_invalidTree = false;
					for (auto& t_createdObject : t_createdObjects)
					{
						if (t_brush->m_priority <= t_createdObject->m_priority)
						{
							if (t_treePos.x > t_createdObject->m_mesh->getTransform().position.x - t_brush->m_radius &&
								t_treePos.x < t_createdObject->m_mesh->getTransform().position.x + t_brush->m_radius &&
								t_treePos.z > t_createdObject->m_mesh->getTransform().position.z - t_brush->m_radius &&
								t_treePos.z < t_createdObject->m_mesh->getTransform().position.z + t_brush->m_radius)
							{
								t_invalidTree = true;
								break;
							}
						}
					}
					if (t_invalidTree == true) {
						continue;
					}

					//Check if tree is near a terrain vertex
					t_treePos.y = m_objectManager.m_terrain->getVertexAtFlatPosition(t_treePos, t_terainRadius).position.y;

					Mesh* t_mesh = new Mesh(object_path, Transform());
					t_mesh->setPosition(t_treePos);
					t_mesh->setScale(glm::vec3(t_randomSize));
					BiomeObject* t_object = new BiomeObject(t_mesh, t_brush->m_radius, t_brush->m_priority);
					t_createdObjects.push_back(t_object);
				}
			}
		}
		if (t_objectID == 1) {
			object_path = "Assets/Bush.obj";
		} else if (t_objectID == 2) {
			object_path = "Assets/Grass.obj";
		}
	}

	for (auto t_createdMesh : t_createdObjects) {
		m_objectManager.addObject(t_createdMesh);
	}
}
