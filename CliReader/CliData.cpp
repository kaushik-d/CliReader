#include <iostream>
#include <iomanip>
#include "CliData.h"

void CliData::clearLayer()
{
	m_layerZ = -1.0;
	m_polylines.clear();
	m_hatches.clear();
	m_layer_area = 0.0;
}

void CliData::printLayer()
{
	std::cout << std::setprecision(2) << std::fixed << "Layer Index: " << m_layerIndex << ", Layer Start Z: " << m_layerZ << ", Layer Part area: " << m_layer_area << std::endl;
}
