#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <iomanip>
#include <string>

#include "CliData.h"
#include "CmdLine.h"
#include "CliHtmlWriter.h"

CliData::CliData()
{
	auto& cmd = CmdLine::Instance();
	m_writeHTML = cmd.isDefined("html");
	if (m_writeHTML) m_htmlLayer = cmd.getInt("html");
}

void CliData::clearLayer()
{
	m_layerZ = -1.0;
	m_polylines.clear();
	m_hatches.clear();
	m_layer_area = 0.0;
	m_partArea.clear();
}

void CliData::layerArea()
{
	m_layer_area = 0;
	for (const auto& polyline : m_polylines) {

		auto partArea = polyline.polygonArea2D();
		partArea *= (m_unit * m_unit);
		auto iter = m_partArea.find(polyline.m_id);
		if (iter != m_partArea.end()) {
			iter->second += partArea;
		}
		else {
			m_partArea[polyline.m_id] = partArea;
		}
		m_layer_area += partArea;
	}
}

void CliData::printLayer()
{
	std::cout << std::setprecision(2) << std::fixed << "Layer Index: " << m_layerIndex << ", Layer Start Z: " << m_layerZ * m_unit
		<< ", Total Layer area = " << std::setprecision(3) << std::fixed << m_layer_area;
	std::for_each(m_partArea.begin(), m_partArea.end(),
		[](std::pair<const int, double>& pair) {
			std::cout << ", Area for part " << pair.first << " = " << std::setprecision(3) << std::fixed << pair.second;
		});
	std::cout << std::endl;

	printLayerInSVG();
}

void CliData::printLayerInSVG()
{
	if (!m_writeHTML) return;

	if (getLayerIndex() == m_htmlLayer) {
		CliHtmlWriter::Instance().addLayer(getLayerIndex(), m_layer_area, m_polylines);
	}
}
