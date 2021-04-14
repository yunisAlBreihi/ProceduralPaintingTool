#include "Biome.h"
#include "BiomeBrush.h"

Biome::Biome(ObjectManager& objectManager, BrushManager& brushManager) : m_objectManager(objectManager), m_brushManager(brushManager) {
	std::vector<BiomeBrush*> t_createdObjects;
	float t_terainRadius = 1.0f;

	const auto& t_brushes = brushManager.getBrushes();

	for (const auto& t_brush : t_brushes) {
		for (auto& t_objectType : t_brush->m_objectProperties) {
			int t_scaledSizeOffsetMin = static_cast<int>(t_objectType->m_sizeOffset.x * 100);
			int t_scaledSizeOffsetMax = static_cast<int>(t_objectType->m_sizeOffset.y * 100);

			int t_scaledPosOffsetXMin = static_cast<int>(t_objectType->m_positionOffsetX.x * 100);
			int t_scaledPosOffsetXMax = static_cast<int>(t_objectType->m_positionOffsetX.y * 100);

			int t_scaledPosOffsetYMin = static_cast<int>(t_objectType->m_positionOffsetY.x * 100);
			int t_scaledPosOffsetYMax = static_cast<int>(t_objectType->m_positionOffsetY.y * 100);

			for (size_t x = 0; x < 10; ++x) {
				for (size_t z = 0; z < 10; ++z) {
					for (size_t i = 0; i < t_objectType->m_frequency; ++i) {
						const float t_randomSize = ((rand() % t_scaledSizeOffsetMax + t_scaledSizeOffsetMin) * 0.01f);
						const float t_offsetX = ((rand() % t_scaledPosOffsetXMax + t_scaledPosOffsetXMin) * 0.01f);
						const float t_offsetZ = ((rand() % t_scaledPosOffsetYMax + t_scaledPosOffsetYMin) * 0.01f);

						glm::vec3 t_treePos = glm::vec3(x * 3 + t_offsetX, 0.0f, z * 3 + t_offsetZ);

						bool t_invalidTree = false;
						for (auto& t_createdObject : t_createdObjects) {
							if (t_objectType->m_priority <= t_createdObject->m_priority) {
								if (t_treePos.x > t_createdObject->m_mesh->getTransform().position.x - t_objectType->m_radius &&
									t_treePos.x < t_createdObject->m_mesh->getTransform().position.x + t_objectType->m_radius &&
									t_treePos.z > t_createdObject->m_mesh->getTransform().position.z - t_objectType->m_radius &&
									t_treePos.z < t_createdObject->m_mesh->getTransform().position.z + t_objectType->m_radius) {
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

						Mesh* t_mesh = new Mesh(t_objectType->m_name.c_str(), Transform());
						t_mesh->setPosition(t_treePos);
						t_mesh->setScale(glm::vec3(t_randomSize));
						BiomeBrush* t_object = new BiomeBrush(t_mesh, t_objectType->m_radius, t_objectType->m_priority);
						t_createdObjects.push_back(t_object);
					}
				}
			}
		}
	}

	for (auto t_createdMesh : t_createdObjects) {
		m_objectManager.addObject(t_createdMesh);
	}
}
