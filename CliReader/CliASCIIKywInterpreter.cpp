#include <iostream>
#include <sstream>
#include <exception>

#include "CliASCIIKywInterpreter.h"
#include "Utils.h"

void CliASCIIKywInterpreter::InterpretHeader()
{
	ReadASCIISection("HEADEREND");
}

void CliASCIIKywInterpreter::InterpretGeometry()
{
	ReadASCIISection("GEOMETRYEND");
}

void CliASCIIKywInterpreter::ParameterValidity(const std::string& kwy, std::vector<std::string>& tokens, size_t minSize, size_t maxSize)
{
	if (minSize < 0) {
		if (tokens.size() < minSize) {
			throw std::runtime_error("Insufficient parameters in kyword " + kwy + ".");
		}
	}
	if (maxSize < 0) {
		if (tokens.size() > maxSize) {
			throw std::runtime_error("Extra parameters in kyword UNITS" + kwy + ".");
		}
	}
}

void CliASCIIKywInterpreter::ParseUnits(const std::string& kwy, std::vector<std::string>& tokens)
{
	ParameterValidity(kwy, tokens, 1, 1);
	m_cliData.setUnit(std::stof(tokens[0]));

}

void CliASCIIKywInterpreter::ParseDimension(const std::string& kwy, std::vector<std::string>& tokens)
{
	ParameterValidity(kwy, tokens, 6, 6);
	std::transform(tokens.begin(), tokens.end(), m_cliData.Dimension().begin(), [](std::string& token) { return std::stof(token); });
}

void CliASCIIKywInterpreter::ParseVersion(const std::string& kwy, std::vector<std::string>& tokens)
{
	ParameterValidity(kwy, tokens, 1, 1);
	m_cliData.setVersion(std::stoi(tokens[0]) / 100);
}

void CliASCIIKywInterpreter::ParseAlign(const std::string& kwy, std::vector<std::string>& tokens)
{
	ParameterValidity(kwy, tokens, 0, 0);
	m_cliData.set32bitAlign();
}

void CliASCIIKywInterpreter::ParseLayer(const std::string& kwy, std::vector<std::string>& tokens)
{
	processPreviousLayer();

	ParameterValidity(kwy, tokens, 1, 1);
	double zLayer = std::stof(tokens[0]);
	m_cliData.incrmentLayerIndex();
	m_cliData.setCurrentLayerZ(zLayer);

	LogLayer(zLayer);
}

void CliASCIIKywInterpreter::ParsePolyline(const std::string& kwy, std::vector<std::string>& tokens)
{
	ParameterValidity(kwy, tokens, 3, -1);
	PolyLine polyline{ std::stoul(tokens[0]), std::stoul(tokens[1]), std::stoul(tokens[2]) };

	size_t maxNumParam = 3 + 2 * polyline.m_nPoints;
	ParameterValidity(kwy, tokens, maxNumParam, maxNumParam);

	std::transform(tokens.begin() + 3, tokens.end(), std::back_inserter(polyline.m_points), [](const std::string& str) {return std::stod(str); });

	LogPolyline(polyline);

	m_cliData.Ploylines().emplace_back(polyline);

}

bool CliASCIIKywInterpreter::isValidKeywordChar(const char c) const {
	// All keywords are written in ASCII upper case notation
	return (c >= 'A' && c <= 'Z');
}

bool CliASCIIKywInterpreter::isValidCliChar(const char c) const {
	// Only the characters A...Z, a...z, , ., 0...9, $ and the separators (section 2.4) are interpreted. All other characters will be ignored.
	// - and + should be valid too
	return (
		(c >= 'A' && c <= 'Z') ||
		(c >= 'a' && c <= 'z') ||
		c == '.' ||
		(c >= '0' && c <= '9') ||
		c == '$' ||
		c == '/' ||
		c == '-' ||
		c == '+' ||
		c == ',' ||
		std::isspace(c)
		);
}

bool CliASCIIKywInterpreter::isPastEndCmdChar(const char c) const {
	return (c == '/' || std::isspace(c));
}

bool CliASCIIKywInterpreter::isLineEnd(const char c) const {
	return (c == '\n' || c == '\r');
}

//
// Skeep comment // ... // starting from the current file pointer
// Returns immediately if file pointer is not at
// Moves the file pointer to end of comment
// throws if no comment clouser is found
//
void CliASCIIKywInterpreter::skipComment() {
	char currC2[2];
	if (m_infile.read(currC2, sizeof(currC2))) {
		if (currC2[0] == '/' && currC2[1] == '/') {
			// Start of comment
			char currC;
			std::stringstream comment;
			comment << currC2[0] << currC2[1];
			while (m_infile.read(&currC, sizeof(currC))) {
				if (currC == '/') {
					if (m_infile.read(&currC, sizeof(currC))) {
						if (currC == '/') {
							// end of keyword
							return;
						}
						else {
							comment << " Invalid comment.";
							throw std::runtime_error(comment.str());
						}
					}
					else {
						comment << " Invalid comment.";
						throw std::runtime_error(comment.str());
					}
				}
				else {
					comment << currC;
				}
			}
			if (m_infile.eof()) {
				comment << " Invalid comment.";
				throw std::runtime_error(comment.str());
			}
		}
		else {
			// not a comment
			m_infile.seekg(-2, std::ios::cur);
			return;
		}
	}
}

void CliASCIIKywInterpreter::InterpretKyw(std::string& keyword, std::string& parameters)
{
	strutl::trim(keyword);
	strutl::trim(parameters);

	std::vector<std::string> parameterToken;
	strutl::split(parameters, ',', parameterToken);

	if (keyword == "BINARY") {
		LogKwyParameters(keyword, parameters);
		m_cliData.setFormat(Format::binary);
	}
	else if (keyword == "ASCII") {
		LogKwyParameters(keyword, parameters);
		m_cliData.setFormat(Format::ascii);
	}
	else if (keyword == "UNITS") {
		LogKwyParameters(keyword, parameters);
		ParseUnits(keyword, parameterToken);
	}
	else if (keyword == "DIMENSION") {
		LogKwyParameters(keyword, parameters);
		ParseDimension(keyword, parameterToken);
	}
	else if (keyword == "VERSION") {
		LogKwyParameters(keyword, parameters);
		ParseVersion(keyword, parameterToken);
	}
	else if (keyword == "ALIGN") {
		LogKwyParameters(keyword, parameters);
		ParseVersion(keyword, parameterToken);
	}
	else if (keyword == "LAYER") {
		ParseLayer(keyword, parameterToken);
	}
	else if (keyword == "POLYLINE") {
		ParsePolyline(keyword, parameterToken);
	}
	else if (keyword == "HEADERSTART")
	{
		LogKwyParameters(keyword, parameters);
	}
	else if (keyword == "HEADEREND")
	{
		LogKwyParameters(keyword, parameters);
	}
	else if (keyword == "GEOMETRYSTART")
	{
		LogKwyParameters(keyword, parameters);
	}
	else if (keyword == "GEOMETRYEND")
	{
		processPreviousLayer();
		LogKwyParameters(keyword, parameters);
	}

	// More to come
}

void CliASCIIKywInterpreter::ReadASCIISection(const std::string endSectionKeyword = "") {

	if (m_infile.is_open()) {
		char currC2[2];
		bool reachedSectionEnd(false);
		while (!reachedSectionEnd && m_infile.read(currC2, sizeof(currC2)))
		{
			bool keyywordRead(false);
			std::string keyword;
			std::string parameters;

			if (currC2[0] == '$' && currC2[1] == '$') {

				keyword.clear();
				parameters.clear();

				// Read the keyword
				char currC;
				while (!reachedSectionEnd && m_infile.read(&currC, sizeof(currC))) {
					if (!isValidCliChar(currC)) {
						continue;
					}
					if (currC == '/') {
						if (m_infile.peek() == '/') {
							// Start of a comment
							m_infile.seekg(-1, std::ios::cur);
							skipComment();
							continue;
						}
					}
					if (isValidKeywordChar(currC))
					{
						keyword += currC;
					}
					if (isPastEndCmdChar(currC) || keyword == endSectionKeyword) {

						keyywordRead = true;
						if (keyword == endSectionKeyword) {
							reachedSectionEnd = true;
						}
						else {
							m_infile.seekg(-1, std::ios_base::cur);
							break;
						}

					}
				}
			}
			else {
				m_infile.seekg(-1, std::ios_base::cur);
			}

			// read parameters
			if (keyywordRead && !reachedSectionEnd) {
				char currC;
				while (!reachedSectionEnd && m_infile.read(&currC, sizeof(currC))) {
					if (isLineEnd(currC)) {
						break;
					}
					if (currC == '/') {
						// May be start of a parameter block
						if (m_infile.peek() == '/') {
							m_infile.seekg(-1, std::ios::cur);
							skipComment();
							continue;
						}
					}
					if (currC == '/') {
						char currC;
						while (!reachedSectionEnd && m_infile.read(&currC, sizeof(currC))) {
							if (currC == '/') {
								// May be start of a parameter block
								if (m_infile.peek() == '/') {
									m_infile.seekg(-1, std::ios::cur);
									skipComment();
									continue;
								}
							} else if (isLineEnd(currC)) {
								m_infile.seekg(-1, std::ios_base::cur);
								break;
							} else if (isValidCliChar(currC) && !isLineEnd(currC)) {
								parameters += currC;
							}
						}
					}
				}
			}

			if (keyywordRead) {
				InterpretKyw(keyword, parameters);
			}

		}
		if (!reachedSectionEnd && !m_infile.eof()) {
			std::cout << "\n *** Err reading file \n";
		}
	}
}
