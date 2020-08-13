#include <iostream>
#include <algorithm>
#include "CliBinKwyInterpreter.h"

void CliBinKwyInterpreter::InterpretHeader()
{
	CliASCIIKywInterpreter::InterpretHeader();
}

void CliBinKwyInterpreter::InterpretBinKwy(BinKeyword kwy)
{
	bool bit32 = m_cliData.is32bitAlign();

	switch (kwy) {
	case BinKeyword::StartLayerLong:
		ParseStartLayerLong();
		break;
	case BinKeyword::StartLayerShort:
		if (!bit32) {
			ParseStartLayer<uint16_t, 1>();
		}
		else {
			ParseStartLayer<uint16_t, 2>();
		}
		break;
	case BinKeyword::StartPolyLineShort:
		if (!bit32) {
			ParseStartPolyLine<uint16_t, 3, uint16_t, 0, 1, 2>();
		}
		else {
			ParseStartPolyLine<uint16_t, 6, uint16_t, 0, 2, 4>();
		}
		break;
	case BinKeyword::StartPolyLineLong:

		ParseStartPolyLine<uint32_t, 3, float, 0, 1, 2>();
		break;
	case BinKeyword::StartHatchesShort:
		if (!bit32) {
			ParseStartHatches<uint16_t, 2, uint16_t, 0, 1>();
		}
		else {
			ParseStartHatches<uint16_t, 4, uint16_t, 0, 2>();
		}
		break;
	case BinKeyword::StartHatchesLong:
		ParseStartHatches<uint32_t, 2, float, 0, 1>();
		break;
	default:
		throw std::runtime_error("Unknown command in binary : " + (int)kwy);
	}
}

void CliBinKwyInterpreter::InterpretGeometry()
{
	if (!m_cliData.is32bitAlign()) {
		uint16_t CommandIndex;
		while (m_infile.read((char*)&CommandIndex, sizeof(CommandIndex))) {

			BinKeyword kwy(static_cast<BinKeyword>(CommandIndex));
			InterpretBinKwy(kwy);

		}
	}
	else {
		uint16_t CommandIndex[2];
		while (m_infile.read((char*)&CommandIndex, sizeof(CommandIndex))) {

			BinKeyword kwy(static_cast<BinKeyword>(CommandIndex[0]));
			InterpretBinKwy(kwy);

		}
	}

	if (m_infile.eof()) processPreviousLayer();
}

void CliBinKwyInterpreter::ParseStartLayerLong() {

	float zLayer;
	if (m_infile.read((char*)&zLayer, sizeof(zLayer))) {

		processPreviousLayer();

		LogLayer(zLayer);
		//m_layerZ.push_back(m_unit * zLayer);
		m_cliData.incrmentLayerIndex();
		m_cliData.setCurrentLayerZ(zLayer);
	}
	else {
		throw std::runtime_error("Error reading ParseStartLayerLong.");
	}
}

template<class T, int N>
void CliBinKwyInterpreter::ParseStartLayer() {

	T zLayer[N];
	if (m_infile.read((char*)&zLayer, sizeof(zLayer))) {

		processPreviousLayer();

		LogLayer(static_cast<double>(zLayer[0]));
		m_cliData.incrmentLayerIndex();
		m_cliData.setCurrentLayerZ(static_cast<double>(zLayer[0]));
	}
	else {
		throw std::runtime_error("Error reading ParseStartLayerShort.");
	}
}

template<class Pram_t, int Param_N, class Data_t, int Id, int Dir, int Npt>
void CliBinKwyInterpreter::ParseStartPolyLine() {

	Pram_t data[Param_N];
	if (!m_infile.read((char*)data, sizeof(data))) {
		throw std::runtime_error("Error reading ParseStartPolyLineShort.");
	}

	PolyLine polyLine{ data[Id], data[Dir], data[Npt] };

	size_t dataSize((size_t)2 * polyLine.m_nPoints);
	std::vector<Data_t> points(dataSize);

	if (m_infile.read((char*)points.data(), (std::streamsize) dataSize * sizeof(Data_t))) {
		polyLine.m_points.insert(polyLine.m_points.begin(), points.begin(), points.end());
	}
	else {
		throw std::runtime_error("Error reading ParseStartPolyLineShort.");
	}

	LogPolyline(polyLine);
	m_cliData.Ploylines().emplace_back(polyLine);
}

template<class Pram_t, int Param_N, class Data_t, int Id, int Npt>
void CliBinKwyInterpreter::ParseStartHatches() {

	Pram_t data[Param_N];
	if (!m_infile.read((char*)data, sizeof(data))) {
		throw std::runtime_error("Error reading ParseStartHatchesShort.");
	}

	Hatch hatch{ data[Id], data[Npt] };

	size_t dataSize((size_t) 4 * data[Npt]);
	std::vector<Data_t> points(dataSize);

	if (m_infile.read((char*)points.data(), (std::streamsize) dataSize * sizeof(Data_t))) {
		hatch.m_points.insert(hatch.m_points.begin(), points.begin(), points.end());
	}
	else {
		throw std::runtime_error("Error reading ParseStartHatchesShort.");
	}

	LogHatches(hatch);

	m_cliData.Hatches().emplace_back(hatch);
}

