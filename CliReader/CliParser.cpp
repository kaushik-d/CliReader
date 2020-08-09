#include <iostream>
#include <cctype>
#include <cstdint>
#include "CliParser.h"
#include "Exceptions.h"
#include "CliBinKwyInterpreter.h"

CliParser::CliParser(const std::filesystem::path& fileName, CliData& cliData)
	: m_fileName(fileName), m_cliData(cliData)
{
}

//
//bool CliParser::isValidKeywordChar(const char c) {
//	// All keywords are written in ASCII upper case notation
//	return (c >= 'A' && c <= 'Z');
//}
//
//bool CliParser::isValidCliChar(const char c) {
//	// Only the characters A...Z, a...z, , ., 0...9, $ and the separators (section 2.4) are interpreted. All other characters will be ignored.
//	return (
//		(c >= 'A' && c <= 'Z') ||
//		(c >= 'a' && c <= 'z') ||
//		c == '.' ||
//		(c >= '0' && c <= '9') ||
//		c == '$' ||
//		c == '/' ||
//		c == ',' ||
//		std::isspace(c)
//		);
//}
//
//bool CliParser::isPastEndCmdChar(const char c) {
//	return (c == '/' || std::isspace(c));
//}
//
//bool CliParser::isLineEnd(const char c) {
//	return (c == '\n' || c == '\r');
//}
//
////
//// Skeep comment // ... // starting from the current file pointer
//// Returns immediately if file pointer is not at the begining of a comment section
//// Moves the file pointer to end of comment
//// throws if no comment clouser is found
////
//void CliParser::skipComment() {
//	char currC2[2];
//	if (m_infile.read(currC2, sizeof(currC2))) {
//		if (currC2[0] == '/' && currC2[1] == '/') {
//			// Start of comment
//			while (m_infile.read(currC2, sizeof(currC2))) {
//				if (currC2[0] == '/' && currC2[1] == '/') {
//					// end of keyword
//					return;
//
//				}
//			}
//			if (m_infile.eof()) {
//				std::cout << "\n *** Err \n";
//			}
//		}
//		else {
//			// not a comment
//			m_infile.seekg(-2, std::ios::cur);
//			return;
//		}
//	}
//}

void CliParser::Initialize()
{
	m_infile.open(m_fileName, std::ios::binary | std::ios::in);
	if (!m_infile.is_open()) {
		throw FileNotFound(m_fileName.string());
	}
}

void CliParser::ParseFile()
{
	setASCIIInterpreter();

	m_interpreter->InterpretHeader();

	if (m_cliData.getFormat() == Format::binary) {
		setBinaryInterpreter();
	}

	m_interpreter->InterpretGeometry();

}

void CliParser::ReadBinarySection() {

	//CliBinKwyInterpreter* intp = new CliBinKwyInterpreter(m_infile);

	//uint16_t CommandIndex;
	//while (m_infile.read((char*)&CommandIndex, sizeof(CommandIndex))) {
	//	switch (CommandIndex) {
	//	case StartLayerLong:
	//		intp->ParseStartLayerLong();
	//		break;
	//	case StartLayerShort:
	//		intp->ParseStartLayerShort();
	//		break;
	//	case StartPolyLineShort:
	//		intp->ParseStartPolyLineShort();
	//		break;
	//	case StartPolyLineLong:
	//		intp->ParseStartPolyLineLong();
	//		break;
	//	case StartHatchesShort:
	//		intp->ParseStartHatchesShort();
	//		break;
	//	case StartHatchesLong:
	//		intp->ParseStartHatchesLong();
	//		break;
	//	default:
	//		throw std::runtime_error("Unknown command in binary : " + CommandIndex);
	//	}
	//}

}

void CliParser::setASCIIInterpreter()
{
	if (m_interpreter != nullptr) {
		if (dynamic_cast<CliASCIIKywInterpreter*>(m_interpreter) != nullptr) {
			return;
		}
		else {
			delete m_interpreter;
		}
	}
	m_interpreter = new CliASCIIKywInterpreter(m_infile, m_cliData);
}

void CliParser::setBinaryInterpreter()
{
	if (m_interpreter != nullptr) {
		if (dynamic_cast<CliBinKwyInterpreter*>(m_interpreter) != nullptr) {
			return;
		}
		else {
			delete m_interpreter;
		}
	}
	m_interpreter = new CliBinKwyInterpreter(m_infile, m_cliData);
}
//
//void CliParser::ReadASCIISection(bool& binaryFormat, const std::string endSectionKeyword = "") {
//
//	if (m_infile.is_open()) {
//		char currC2[2];
//		bool reachedSectionEnd(false);
//		while (!reachedSectionEnd && m_infile.read(currC2, sizeof(currC2)))
//		{
//			bool keyywordRead(false);
//			std::string keyword;
//			std::string parameters;
//
//			if (currC2[0] == '$' && currC2[1] == '$') {
//
//				keyword.clear();
//				parameters.clear();
//
//				char currC;
//				while (!reachedSectionEnd && m_infile.read(&currC, sizeof(currC))) {
//					if (!isValidCliChar(currC)) {
//						continue;
//					}
//					if (currC == '/') {
//						if (m_infile.peek() == '/') {
//							// Start of a comment
//							m_infile.seekg(-1, std::ios::cur);
//							skipComment();
//						}
//					}
//					if (isValidKeywordChar(currC))
//					{
//						keyword += currC;
//					}
//					if (keyword == "BINARY") {
//						binaryFormat = true;
//					}
//					if (isPastEndCmdChar(currC) || keyword == endSectionKeyword) {
//						std::cout << "Keyword : " << keyword << std::endl;
//						keyywordRead = true;
//						if (keyword == endSectionKeyword) {
//							reachedSectionEnd = true;
//						}
//						else {
//							m_infile.seekg(-1, std::ios_base::cur);
//							break;
//						}
//
//					}
//				}
//			}
//			else {
//				m_infile.seekg(-1, std::ios_base::cur);
//			}
//
//			if (keyywordRead && !reachedSectionEnd) {
//				/*
//				char currC;
//				while (!reachedSectionEnd && m_infile.read(&currC, sizeof(currC))) {
//					if (isLineEnd(currC)) {
//						break;
//					}
//					if (currC == '/') {
//						// May be start of a parameter block
//						if (m_infile.peek() == '/') {
//							m_infile.seekg(-1, std::ios::cur);
//							skipComment();
//							continue;
//						}
//					}
//					if (currC == '/') {
//						char currC;
//						while (!reachedSectionEnd && m_infile.read(&currC, sizeof(currC))) {
//							if (isValidCliChar(currC) && !isLineEnd(currC)) {
//								parameters += currC;
//							}
//							if (isLineEnd(currC)) {
//								std::cout << "Parameter : " << parameters << std::endl;
//								m_infile.seekg(-1, std::ios_base::cur);
//								break;
//							}
//						}
//					}
//
//				}
//				*/
//			}
//
//		}
//		if (!reachedSectionEnd && !m_infile.eof()) {
//			std::cout << "\n *** Err reading file \n";
//		}
//	}
//}
