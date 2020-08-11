#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <iomanip>
#include <string>

#include "CliData.h"
#include "CmdLine.h"

CliData::CliData()
{
	auto& cmd = CmdLine::Instance();
	m_writeHTML = cmd.isDefined("html");
	if (m_writeHTML) {
		m_path = CmdLine::Instance().getString("f");
		m_path.make_preferred();
		m_cliFileName = m_path.filename();
	}
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

		polyline.polygonBBox(m_dimFromLayer);

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
<body>

<svg width=)"+ std::to_string(widthSVG) + " height="+ std::to_string(heightSVG) + R"(>
)";
	//100,10 40,198 190,78 10,78 160,198
	std::string ppts;
	for (const auto& polyline : m_polylines) {
			ppts += R"(
<polyline points=")";
			for (int i = 0; i < 2 * polyline.m_nPoints; i++) {
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
	//	}
	}

	std::string htmlTail =
		R"("</svg>
</body>
<script>
function hideSVG() {
  var style = document.getElementById("myRect").style.display;
  if(style === "none") {
    matches = document.querySelectorAll('[id=myRect]');
    for (const match of matches.values()) {
	  	match.style.display = "block";
		};
  }
  else {
    matches = document.querySelectorAll('[id=myRect]');
    for (const match of matches.values()) {
	  	match.style.display = "none";
		};
  }
</script>
</html>
)";

	std::string html = htmlHead + ppts + htmlTail;

	std::filesystem::path htmlFileName( "Layer_" + std::to_string(getLayerIndex()) + "_");
	htmlFileName += m_cliFileName;
	htmlFileName.replace_extension("html");

	m_path.replace_filename(htmlFileName);

	std::ofstream htmlFile(m_path);

	htmlFile << html;

	htmlFile.close();

}
