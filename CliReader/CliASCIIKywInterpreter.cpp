#include <iostream>
#include "CliASCIIKywInterpreter.h"

void CliASCIIKywInterpreter::InterpretHeader()
{
	ReadASCIISection("HEADEREND");
}

void CliASCIIKywInterpreter::InterpretGeometry()
{
	ReadASCIISection("GEOMETRYEND");
}

void CliASCIIKywInterpreter::ParseStartLayerLong()
{
}

void CliASCIIKywInterpreter::ParseStartLayerShort()
{
}

void CliASCIIKywInterpreter::ParseStartPolyLineShort()
{
}

void CliASCIIKywInterpreter::ParseStartPolyLineLong()
{
}

void CliASCIIKywInterpreter::ParseStartHatchesShort()
{
}

void CliASCIIKywInterpreter::ParseStartHatchesLong()
{
}


bool CliASCIIKywInterpreter::isValidKeywordChar(const char c) const {
	// All keywords are written in ASCII upper case notation
	return (c >= 'A' && c <= 'Z');
}

bool CliASCIIKywInterpreter::isValidCliChar(const char c) const {
	// Only the characters A...Z, a...z, , ., 0...9, $ and the separators (section 2.4) are interpreted. All other characters will be ignored.
	return (
		(c >= 'A' && c <= 'Z') ||
		(c >= 'a' && c <= 'z') ||
		c == '.' ||
		(c >= '0' && c <= '9') ||
		c == '$' ||
		c == '/' ||
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
			while (m_infile.read(currC2, sizeof(currC2))) {
				if (currC2[0] == '/' && currC2[1] == '/') {
					// end of keyword
					return;

				}
			}
			if (m_infile.eof()) {
				std::cout << "\n *** Err \n";
			}
		}
		else {
			// not a comment
			m_infile.seekg(-2, std::ios::cur);
			return;
		}
	}
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
						}
					}
					if (isValidKeywordChar(currC))
					{
						keyword += currC;
					}
					if (keyword == "BINARY") {
						m_cliData.setFormat(Format::binary);
					}
					if (isPastEndCmdChar(currC) || keyword == endSectionKeyword) {
						std::cout << "Keyword : " << keyword << std::endl;
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
							if (isValidCliChar(currC) && !isLineEnd(currC)) {
								parameters += currC;
							}
							if (isLineEnd(currC)) {
								std::cout << "Parameter : " << parameters << std::endl;
								m_infile.seekg(-1, std::ios_base::cur);
								break;
							}
						}
					}

				}
			}

		}
		if (!reachedSectionEnd && !m_infile.eof()) {
			std::cout << "\n *** Err reading file \n";
		}
	}
}
