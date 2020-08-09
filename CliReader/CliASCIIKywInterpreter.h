#pragma once
#include <string>
#include "CliKwyInterpreter.h"
#include "CliData.h"

class CliASCIIKywInterpreter : public CliKwyInterpreter
{
public:
	CliASCIIKywInterpreter(std::ifstream& infile, CliData& cliData) : CliKwyInterpreter(infile, cliData) {}

	virtual void InterpretHeader();
	virtual void InterpretGeometry();

private:

	bool isValidKeywordChar(const char c) const;
	bool isValidCliChar(const char c) const;
	bool isPastEndCmdChar(const char c) const;
	bool isLineEnd(const char c) const;
	void skipComment();
	void ReadASCIISection(const std::string endSectionKeyword);

	void ParseStartLayerLong();
	void ParseStartLayerShort();
	void ParseStartPolyLineShort();
	void ParseStartPolyLineLong();
	void ParseStartHatchesShort();
	void ParseStartHatchesLong();

	std::string parameters;
};

