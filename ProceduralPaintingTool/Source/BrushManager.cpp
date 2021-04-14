#include "BrushManager.h"

BrushProperty& BrushManager::CreateBrush() {
	m_brushProperties.push_back(new BrushProperty());
	m_currentBrushProperty = m_brushProperties[m_brushProperties.size() - 1];
	return *m_currentBrushProperty;
}

BrushProperty& BrushManager::setCurrentBrush(int index) {
	m_currentBrushProperty = m_brushProperties[index];
	return *m_currentBrushProperty;
}
