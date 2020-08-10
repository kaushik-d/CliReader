#include "PolyLine.h"

double PolyLine::polygonArea2D() const
{
	//  Input:  PointsCoord size = [2*(iNumVertexPolygon+1)] = an array of n+1 vertex {x,y} points with n+1th vertex = 0 th vertex
	//  Return: the (double) area of the polygon

	if (m_dir == open) return 0.0;

	const int iNumVertexPolygon = m_points.size() / 2 - 1;
	if (iNumVertexPolygon < 3) {
		return 0.0;
	}

	double area(0);

	for (int i = 1, j = 2, k = 0; i < iNumVertexPolygon; i++, j++, k++) {

		area += m_points[2 * i] * (m_points[2 * j + 1] - m_points[2 * k + 1]);
	}

	area += m_points[2 * iNumVertexPolygon] * (m_points[3] - m_points[2 * (iNumVertexPolygon - 1) + 1]);

	area = area / 2.0;

	if (m_dir == counterClockwise && area < 0.0) {
		area = -area;
	}
	else if (m_dir == clockwise && area > 0.0) {
		area = -area;
	}

	return area;
}
