#pragma once
#include <fstream>
#include <filesystem>
#include "CliData.h"
#include "CliKwyInterpreter.h"

class CliParser
{

	enum commadIndex {
		StartLayerLong = 127,
		StartLayerShort = 128,
		StartPolyLineShort = 129,
		StartPolyLineLong = 130,
		StartHatchesShort = 131,
		StartHatchesLong = 132
	};

	struct polyLine {
		unsigned int id;
		unsigned int dir;
		unsigned int nPoints;
		std::vector<double> points;
	};

public:

	CliParser(const std::filesystem::path& fileName, CliData& cliData);
	virtual ~CliParser() {};

	void Initialize();
	void ParseFile();

	//void ParseStartLayerLong();
	//void ParseStartLayerShort();
	//void ParseStartPolyLineShort();
	//void ParseStartPolyLineLong();
	//void ParseStartHatchesShort();
	//void ParseStartHatchesLong();
	void ReadBinarySection();
	//void ReadASCIISection(bool& binaryFormat, const std::string endSectionKeyword);

private:

	//bool isValidKeywordChar(const char c);
	//bool isValidCliChar(const char c);
	//bool isPastEndCmdChar(const char c);
	//bool isLineEnd(const char c);
	//void skipComment();

	CliKwyInterpreter* m_interpreter{ nullptr };

	void setASCIIInterpreter();
	void setBinaryInterpreter();

	std::ifstream m_infile;
	std::filesystem::path m_fileName;
	CliData& m_cliData;

	std::vector<double> m_layerZ;
	std::vector< polyLine > m_polylines;

	double m_unit{ 1 };
};

