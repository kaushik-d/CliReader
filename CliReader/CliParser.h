#pragma once
#include <fstream>
#include <filesystem>

class CliParser
{
public:
	CliParser(const std::filesystem::path& fileName);
	virtual ~CliParser() {};

	void Initialize();
	void ReadASCIISection(bool& binaryFormat, const std::string endSectionKeyword);

private:

	bool isValidKeywordChar(const char c);
	bool isValidCliChar(const char c);
	bool isPastEndCmdChar(const char c);
	bool isLineEnd(const char c);
	void skipComment();

	std::ifstream m_infile;
	std::filesystem::path m_fileName;
};

