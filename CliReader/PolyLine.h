#pragma once
#include <vector>

class PolyLine
{
public:
	unsigned int m_id;
	unsigned int m_dir;
	unsigned int m_nPoints;
	std::vector<double> m_points;
};

