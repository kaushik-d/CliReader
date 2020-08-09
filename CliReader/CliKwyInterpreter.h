#pragma once
#include <fstream>
#include <filesystem>
#include "PolyLine.h"
#include "Hatch.h"

class CliKwyInterpreter
{
public:

	CliKwyInterpreter(std::ifstream& infile) : m_infile(infile) {}


protected:

	std::ifstream& m_infile;
	std::filesystem::path m_fileName;

	double m_unit{ 1 };
	std::vector<double> m_layerZ;
	std::vector<PolyLine> m_polylines;
	std::vector<Hatch> m_hatches;


protected:
	void LogPolyline(const PolyLine& polyLine) const;
	void LogHatches(const Hatch& hatch) const;
	void LogLayer(double zLayer) const;
};

