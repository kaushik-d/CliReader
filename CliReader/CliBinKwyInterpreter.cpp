#include <iostream>
#include <algorithm>
#include "CliBinKwyInterpreter.h"
#include "CliKwyLib.h"

void CliBinKwyInterpreter::InterpretHeader()
{
	CliASCIIKywInterpreter::InterpretHeader();
}

void CliBinKwyInterpreter::InterpretGeometry()
{
	uint16_t CommandIndex;
	while (m_infile.read((char*)&CommandIndex, sizeof(CommandIndex))) {

		BinKeyword kwy(static_cast<BinKeyword>(CommandIndex));

		switch (kwy) {
		case BinKeyword::StartLayerLong:
			ParseStartLayerLong();
			break;
		case BinKeyword::StartLayerShort:
			ParseStartLayerShort();
			break;
		case BinKeyword::StartPolyLineShort:
			ParseStartPolyLineShort();
			break;
		case BinKeyword::StartPolyLineLong:
			ParseStartPolyLineLong();
			break;
		case BinKeyword::StartHatchesShort:
			ParseStartHatchesShort();
			break;
		case BinKeyword::StartHatchesLong:
			ParseStartHatchesLong();
			break;
		default:
			throw std::runtime_error("Unknown command in binary : " + CommandIndex);
		}
	}
}

void CliBinKwyInterpreter::ParseStartLayerLong() {
	//
	// compute area and clear polylines of last layer
	//

	if (!m_polylines.empty()) {
		// get area
		m_polylines.clear();
	}

	float zLayer;
	if (m_infile.read((char*)&zLayer, sizeof(zLayer))) {

		LogLayer(zLayer);
		m_layerZ.push_back(m_unit * zLayer);
	}
	else {
		throw std::runtime_error("Error reading ParseStartLayerLong.");
	}
}

void CliBinKwyInterpreter::ParseStartLayerShort() {
	uint16_t zLayer;
	if (m_infile.read((char*)&zLayer, sizeof(zLayer))) {

		LogLayer(zLayer);
		m_layerZ.push_back(m_unit * zLayer);
	}
	else {
		throw std::runtime_error("Error reading ParseStartLayerShort.");
	}
}

void CliBinKwyInterpreter::ParseStartPolyLineShort() {
	enum { id = 0, dir, nPoints };
	uint16_t data[3];
	if (!m_infile.read((char*)data, sizeof(data))) {
		throw std::runtime_error("Error reading ParseStartPolyLineShort.");
	}

	PolyLine polyLine{ data[id], data[dir], data[nPoints] };

	size_t dataSize(2 * polyLine.m_nPoints);
	std::vector<uint16_t> points(dataSize);

	if (m_infile.read((char*)points.data(), (std::streamsize) dataSize * sizeof(uint16_t))) {
		polyLine.m_points.insert(polyLine.m_points.begin(), points.begin(), points.end());
	}
	else {
		throw std::runtime_error("Error reading ParseStartPolyLineShort.");
	}

	LogPolyline(polyLine);

	for (auto& point : polyLine.m_points) {
		point *= m_unit;
	}

	m_polylines.emplace_back(polyLine);
}

void CliBinKwyInterpreter::ParseStartPolyLineLong()
{
	enum { id = 0, dir, nPoints };
	uint32_t data[3];
	if (!m_infile.read((char*)data, sizeof(data))) {
		throw std::runtime_error("Error reading ParseStartPolyLineLong.");
	}

	PolyLine polyLine{ data[id], data[dir], data[nPoints] };

	size_t dataSize(2 * polyLine.m_nPoints);
	std::vector<float> points(dataSize);

	if (m_infile.read((char*)points.data(), (std::streamsize) dataSize * sizeof(uint32_t))) {
		polyLine.m_points.insert(polyLine.m_points.begin(), points.begin(), points.end());
	}
	else {
		throw std::runtime_error("Error reading ParseStartPolyLineLong.");
	}

	LogPolyline(polyLine);

	for (auto& point : polyLine.m_points) {
		point *= m_unit;
	}

	m_polylines.emplace_back(polyLine);
}

void CliBinKwyInterpreter::ParseStartHatchesShort() {
	enum { id = 0, nPoints };
	uint16_t data[2];
	if (!m_infile.read((char*)data, sizeof(data))) {
		throw std::runtime_error("Error reading ParseStartHatchesShort.");
	}

	Hatch hatch{ data[id], data[nPoints] };

	size_t dataSize(4 * data[nPoints]);
	std::vector<uint16_t> points(dataSize);

	if (m_infile.read((char*)points.data(), (std::streamsize) dataSize * sizeof(uint16_t))) {
		hatch.m_points.insert(hatch.m_points.begin(), points.begin(), points.end());
	}
	else {
		throw std::runtime_error("Error reading ParseStartHatchesShort.");
	}

	LogHatches(hatch);

	for (auto& point : hatch.m_points) {
		point *= m_unit;
	}

	m_hatches.emplace_back(hatch);
}

void CliBinKwyInterpreter::ParseStartHatchesLong() {
	enum { id = 0, nPoints };
	uint32_t data[2];
	if (!m_infile.read((char*)data, sizeof(data))) {
		throw std::runtime_error("Error reading ParseStartHatchesLong.");
	}

	Hatch hatch{ data[id], data[nPoints] };

	size_t dataSize(4 * data[nPoints]);
	std::vector<float> points(dataSize);

	if (m_infile.read((char*)points.data(), (std::streamsize) dataSize * sizeof(uint32_t))) {
		hatch.m_points.insert(hatch.m_points.begin(), points.begin(), points.end());
	}
	else {
		throw std::runtime_error("Error reading ParseStartHatchesLong.");
	}

	LogHatches(hatch);

	for (auto& point : hatch.m_points) {
		point *= m_unit;
	}

	m_hatches.emplace_back(hatch);
}

