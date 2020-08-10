#pragma once
#include <fstream>
#include <filesystem>
#include "CliData.h"

class CliKwyInterpreter
{
public:

	CliKwyInterpreter(std::ifstream& infile, CliData& cliData);

	virtual void InterpretHeader() = 0;
	virtual void InterpretGeometry() = 0;

	void setLogLevel(int logLevel) { m_logLevel = logLevel; }

protected:

	std::filesystem::path m_fileName;
	std::ifstream& m_infile;

	CliData& m_cliData;

	int m_logLevel{ 0 };

	void LogKwyParameters(const std::string& kwy, const std::string& params);
	void LogPolyline(const PolyLine& polyLine) const;
	void LogHatches(const Hatch& hatch) const;
	void processPreviousLayer();
	void LogLayer(double zLayer) const;
};

