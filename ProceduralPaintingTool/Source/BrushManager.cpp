#include "BrushManager.h"
#include "IOHandler.h"
#include "globals.h"

void BrushManager::start() {
	IOHandler::loadJson_brush(globals::g_saveNameBrushes, *this);
}

void BrushManager::quit() {
	IOHandler::saveJson_brush(globals::g_saveNameBrushes, *this);
}

BrushProperty& BrushManager::createBrush() {
	BrushProperty* t_brush = new BrushProperty();
	t_brush->m_id = ++m_brushIdCount;
	m_brushProperties.push_back(t_brush);
	m_currentBrushProperty = m_brushProperties[m_brushProperties.size() - 1];
	return *m_currentBrushProperty;
}

void BrushManager::addBrush(BrushProperty* brushProperty) {
	m_brushProperties.push_back(brushProperty);
	m_currentBrushProperty = m_brushProperties[m_brushProperties.size() - 1];
}

BrushProperty& BrushManager::setCurrentBrush(const int index) {
	m_currentBrushProperty = m_brushProperties[index];
	return *m_currentBrushProperty;
}

void BrushManager::removeVertexFromOtherBrushesAtPosition(const glm::vec3& position) {
	for (const auto& t_brush : m_brushProperties) {
		if (t_brush->m_id != m_currentBrushProperty->m_id) {
			t_brush->removeVertexAtPosition(position);
		}
	}
}

void BrushManager::removeVerticesFromOtherBrushes() {
	for (const auto& t_brush : m_brushProperties) {
		if (t_brush->m_id != m_currentBrushProperty->m_id) {
			t_brush->removeAllVertices();
		}
	}
}
