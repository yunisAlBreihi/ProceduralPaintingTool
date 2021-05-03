#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Transform.h"
#include "ObjectManager.h"
#include "BrushProperty.h"
#include "BrushManager.h"

class GUIBrushProperties
{
private:
	ObjectManager& m_objectManager;
	BrushManager& m_brushManager;
	Transform m_transform;

	BiomeObjectProperty* m_currentObjectProperty = nullptr;
	BrushProperty* m_currentBrush = nullptr;

	int m_brushCount = 0;
	int m_currentBrushIndex = 0;

	static const int m_bufferSize = 512;
	char m_buffer[m_bufferSize] = "";

private:
	//UI variables
	const char* m_GUIName= "Brush Properties";

	const char* m_seedName = "Seed";

	const char* m_brushRadiusName = "Brush Radius";
	const float m_brushRadiusMin = 1.0f;
	const float m_brushRadiusMax = 4.0f;

	const char* m_createBrushName = "Create Brush";
	const char* m_brushNamingPopupName  = "brush_naming_popup";
	const char* m_createBrushInputName = "Brush Name";

	const char* m_deleteBrushName = "Delete Current Brush";

	const char* m_headerBrushColorName = "Brush Color";
	const char* m_colorPickerName = "Brush Vertex Color";

	const char* m_fillTerrainColorName = "Fill terrain with color";

	const char* m_createBiomeName = "Create Biome";

	const char* m_clearMeshesName = "Clear Meshes";

	const char* m_RemoveTerrainColorsName = "Remove Terrain Colors";

	const char* m_headerPriopertiesName = "Biome Properties";

	const char* m_sliderSizeRangeName = "Size Range";
	const float m_sizeRangeMin = 0.1f;
	const float m_sizeRangeMax = 0.1f;

	const char* m_sliderPosRangeXName = "Position Range X";
	const float m_posRangeXMin = -10.0f;
	const float m_posRangeXMax = 10.0f;

	const char* m_sliderPosRangeYName = "Position Range Y";
	const float m_posRangeYMin = -10.0f;
	const float m_posRangeYMax = 10.0f;

	const char* m_sliderRotRangeXName = "Rotation Range X";
	const float m_rotRangeXMin = -360.0f;
	const float m_rotRangeXMax = 360.0f;

	const char* m_sliderRotRangeYName = "Rotation Range Y";
	const float m_rotRangeYMin = -360.0f;
	const float m_rotRangeYMax = 360.0f;

	const char* m_sliderRotRangeZName = "Rotation Range Z";
	const float m_rotRangeZMin = -360.0f;
	const float m_rotRangeZMax = 360.0f;

	const char* m_objectRadiusName = "Object Radius";
	const float m_objectRadiusMin = 0.1f;
	const float m_objectRadiusMax = 10.0f;

	const char* m_frequencyName = "Frequency";
	const int m_frequencyMin = 1;
	const int m_frequencyMax = 20;

	const char* m_priorityName = "Priority";
	const int m_priorityMin = 0;
	const int m_priorityMax = 20;

	const char* m_objectListName = "Object List";

	const char* m_importMeshName = "Import Mesh";

	const char* m_dialogueTitleName = "TextureOpenDialog";
	const char* m_dialogueName = "TextureOpenDialog";
	const char* m_dialogueTypes = "Mesh File (*.obj){.obj},.*";

	const char* m_deleteCurrentMeshName = "Delete Current Mesh";


private:
	void showBrushButtons(const std::vector<BrushProperty*>& brushes, int& item);
	void collapsingPropertyMenu(int& item);
	void separatorCreator(int count);

public:
	GUIBrushProperties(ObjectManager& objectManager, BrushManager& brushManager);
	void start();
	void update();
	void render(GLFWwindow* window);
};