#pragma once
#include <fstream>
#include <filesystem>
#include "CliData.h"

class CliKwyInterpreter
{
public:

	CliKwyInterpreter(std::ifstream& infile, CliData& cliData) 
		: m_infile(infile), m_cliData(cliData) {}

	virtual void InterpretHeader() = 0;
	virtual void InterpretGeometry() = 0;

	//virtual void ParseStartLayerLong() = 0;
	//virtual void ParseStartLayerShort() = 0;
	//virtual void ParseStartPolyLineShort() = 0;
	//virtual void ParseStartPolyLineLong() = 0;
	//virtual void ParseStartHatchesShort() = 0;
	//virtual void ParseStartHatchesLong() = 0;

protected:

	std::ifstream& m_infile;
	CliData& m_cliData;

	std::filesystem::path m_fileName;

	//double m_unit{ 1 };
	//std::vector<double> m_layerZ;
	//std::vector<PolyLine> m_polylines;
	//std::vector<Hatch> m_hatches;


protected:
	void LogPolyline(const PolyLine& polyLine) const;
	void LogHatches(const Hatch& hatch) const;
	void LogLayer(double zLayer) const;
};

