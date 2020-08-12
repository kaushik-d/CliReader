#include <fstream>
#include <sstream> 
#include <iomanip>
#include "CliHtmlWriter.h"
#include "CmdLine.h"

void CliHtmlWriter::Inialize()
{
	m_path = CmdLine::Instance().getString("f");
	m_path.make_preferred();
	m_path.replace_extension("html");
}

void CliHtmlWriter::addLayer(int layerId, double area, const std::vector<PolyLine>& polylines)
{
	m_area.push_back(area);
	m_polylines.insert(std::make_pair(layerId,polylines));

	for (const auto& polyline : polylines) {
		polyline.polygonBBox(m_dimFromLayer);
	}
}

void CliHtmlWriter::CliWriteHtmlFile() 
{

	double BBoxW = m_dimFromLayer[2] - m_dimFromLayer[0];
	double BBoxH = m_dimFromLayer[3] - m_dimFromLayer[1];
	double ratio = BBoxH / BBoxW;
	double widthSVG = 500;
	double heightSVG = widthSVG * ratio;
	double unit = widthSVG / BBoxW;

	std::string htmlHead =
		R"(
<!DOCTYPE html>
<html>
<body>)";

	std::string ppts;
	ppts += R"(<svg width= )" + std::to_string(widthSVG) + " height=" + std::to_string(heightSVG) + R"(>
)";

	int layerId(0);
	for (const auto& polylines : m_polylines) {

		for (const auto& polyline : polylines.second) {
			ppts += R"(
<polyline id= Layer_)" + std::to_string(layerId) + R"( points=")";
			for (unsigned int i = 0; i < 2 * polyline.m_nPoints; i++) {
				double coord = (double)(polyline.m_points[i] - (i % 2 == 0 ? m_dimFromLayer[0] : m_dimFromLayer[1])) * unit;
				ppts += std::to_string(coord);
				if (i % 2 == 0 && i != 2 * polyline.m_nPoints - 1) {
					ppts += ",";
				}
				else {
					ppts += " ";
				}
			}
			switch (polyline.m_dir) {
			case polyline.open:
				ppts += R"(" style="fill:white;stroke:purple;stroke-width:1;" />
)";
				break;
			case polyline.clockwise:
				ppts += R"(" style="fill:white;stroke:blue;stroke-width:1;" />
)";
				break;
			case polyline.counterClockwise:
				ppts += R"(" style="fill:white;stroke:red;stroke-width:1;" />
)";
				break;
			}
		}
		std::stringstream  area;
		area << std::setprecision(3) << std::fixed << m_area[layerId];
		ppts += R"(<text x = "25" y = "25" fill = "bloack">)" + area.str() + R"( mm^2</text>)";
		layerId++;
	}
	ppts += R"(</svg>)";

	std::string htmlTail =
R"(</body>
</html>
)";

	std::string html = htmlHead + ppts + htmlTail;

	std::ofstream htmlFile(m_path);

	htmlFile << html;

	htmlFile.close();

}
