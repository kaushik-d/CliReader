#include <iostream>
#include "CliKwyInterpreter.h"


void CliKwyInterpreter::LogLayer(double zLayer) const
{
	std::cout << "Keyword : START LAYER" << std::endl;
	std::cout << "Parameter : " << zLayer << std::endl;
}

void CliKwyInterpreter::LogPolyline(const PolyLine& polyLine) const
{
	std::cout << "Keyword : START POLYLINE" << std::endl;
	std::cout << "Parameter : " << polyLine.m_id << "," << polyLine.m_dir << "," << polyLine.m_nPoints << ",";
	std::for_each(polyLine.m_points.begin(), polyLine.m_points.end(), [](const auto& pt) { std::cout << pt << ","; });
	std::cout << std::endl;
}

void CliKwyInterpreter::LogHatches(const Hatch& hatch) const
{
	std::cout << "Keyword : START POLYLINE SHORT" << std::endl;
	std::cout << "Parameter : " << hatch.m_id << "," << hatch.m_nPoints << ",";
	std::for_each(hatch.m_points.begin(), hatch.m_points.end(), [](const auto& pt) { std::cout << pt << ","; });
	std::cout << std::endl;
}
