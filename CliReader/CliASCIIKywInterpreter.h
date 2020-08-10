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
	void InterpretKyw(std::string& keyword, std::string& parameters);
	void ReadASCIISection(const std::string endSectionKeyword);

	void ParameterValidity(const std::string& kwy, std::vector<std::string>& tokens, int minSize, int maxSize);
	void ParseUnits(const std::string& kwy, std::vector<std::string>& tokens);
	void ParseVersion(const std::string& kwy, std::vector<std::string>& tokens);
	void ParseAlign(const std::string& kwy, std::vector<std::string>& tokens);
	void ParseLayer(const std::string& kwy, std::vector<std::string>& tokens);
	void ParsePolyline(const std::string& kwy, std::vector<std::string>& tokens);

	std::string parameters;
};

