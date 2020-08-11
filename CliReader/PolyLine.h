#pragma once
#include <vector>

class PolyLine
{
public:

	enum dir {
		clockwise = 0, // internal
		counterClockwise = 1, // external
		open = 2 // no solid
	};

	unsigned int m_id;
	unsigned int m_dir;
	unsigned int m_nPoints;
	std::vector<double> m_points;
	double polygonArea2D() const;
	void polygonBBox(std::array<double, 4>& BBox2d) const;
};

