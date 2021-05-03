#include "Biome.h"
#include "BiomeObject.h"

Biome::Biome(ObjectManager& objectManager, BrushManager& brushManager) : m_objectManager(objectManager), m_brushManager(brushManager) {
	createBiome();
}

void Biome::createBiome() {
	std::vector<BiomeObject*> t_createdObjects;
	float t_terrainRadius = 1.0f;

	const auto& t_brushes = m_brushManager.getBrushes();

	//Get the terrain size to determine the grid size
	Mesh& t_terrain = *m_objectManager.m_terrain;
	const Vertex& t_lowestTerrainVertex = t_terrain.getLowestVertexPositionFlat();
	glm::vec2 t_lowestPosition{ t_lowestTerrainVertex.position.x, t_lowestTerrainVertex.position.z };

	const Vertex& t_highestTerrainVertex = t_terrain.getHighestVertexPositionFlat();
	glm::vec2 t_highestPosition{ t_highestTerrainVertex.position.x, t_highestTerrainVertex.position.z };

	const int t_minCountX = static_cast<int>(t_lowestPosition.x);
	const int t_minCountZ = static_cast<int>(t_lowestPosition.y);

	const int t_maxCountX = static_cast<int>(t_highestPosition.x);
	const int t_maxCountZ = static_cast<int>(t_highestPosition.y);

	int t_brushCounter = 0;
	for (auto& t_brush : t_brushes) {
		if (t_brushCounter == 0) {
			t_brushCounter++;
			continue;
		}
		for (auto& t_objectType : t_brush->m_objectProperties) {

			srand(globals::g_seed);

			//
			for (int x = t_minCountX; x < t_maxCountX; ++x) {
				for (int z = t_minCountZ; z < t_maxCountZ; ++z) {
					for (int i = 0; i < t_objectType->m_frequency; ++i) {

						float t_randomSize = globals::randRangef(t_objectType->m_sizeOffset);
						float t_posOffsetX = globals::randRangef(t_objectType->m_positionOffsetX);
						float t_posOffsetZ = globals::randRangef(t_objectType->m_positionOffsetY);

						float t_rotOffsetX = globals::randRangef(t_objectType->m_rotationOffsetX);
						float t_rotOffsetY = globals::randRangef(t_objectType->m_rotationOffsetY);
						float t_rotOffsetZ = globals::randRangef(t_objectType->m_rotationOffsetZ);


						glm::vec3 t_treePos = glm::vec3(x + t_posOffsetX, 0.0f, z + t_posOffsetZ);

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
						Vertex* t_vertex = globals::getVertexAtPositionFlat(t_brush->m_usedVertices, t_treePos, t_terrainRadius);
						if (t_vertex != nullptr) {
							//If so, create a mesh of the object type.
							t_treePos.y = t_vertex->position.y;
							Mesh* t_mesh = new Mesh(("Assets/" + t_objectType->m_name).c_str(), Transform());
							t_mesh->setPosition(t_treePos);
							t_mesh->addRotation(t_rotOffsetX, globals::RIGHT);
							t_mesh->addRotation(t_rotOffsetY, globals::UP);
							t_mesh->addRotation(t_rotOffsetZ, globals::FORWARD);
							t_mesh->setScale(glm::vec3(t_randomSize));
							BiomeObject* t_object = new BiomeObject(t_mesh, t_objectType->m_radius, t_objectType->m_priority);

							//Adds the mesh to the list of objects to be created
							t_createdObjects.push_back(t_object);
						}
					}
				}
			}
		}
		t_brush->m_colorIsSet = true;
	}

	//Add all created meshes to the object manager
	for (auto t_createdMesh : t_createdObjects) {
		m_objectManager.addObject(t_createdMesh);
	}
}
