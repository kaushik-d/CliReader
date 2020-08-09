// CliReader.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <filesystem>
#include <cctype>

#include "CmdLine.h"
#include "CliParser.h"

/*
bool isValidKeywordChar(const char c) {
	// All keywords are written in ASCII upper case notation
	return (c >= 'A' && c <= 'Z');
}

bool isValidCliChar(const char c) {
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

bool isPastEndCmdChar(const char c) {
	return (c == '/' || std::isspace(c));
}

bool isLineEnd(const char c) {
	return (c == '\n' || c == '\r');
}

//
// Skeep comment // ... // starting from the current file pointer
// Returns immediately if file pointer is not at the begining of a comment section
// Moves the file pointer to end of comment
// throws if no comment clouser is found
//
void skipComment(std::ifstream& infile) {
	char currC2[2];
	if (infile.read(currC2, sizeof(currC2))) {
		if (currC2[0] == '/' && currC2[1] == '/') {
			// Start of comment
			while (infile.read(currC2, sizeof(currC2))) {
				if (currC2[0] == '/' && currC2[1] == '/') {
					// end of keyword
					return;

				}
			}
			if (infile.eof()) {
				std::cout << "\n *** Err \n";
			}
		}
		else {
			// not a comment
			infile.seekg(-2, std::ios::cur);
			return;
		}
	}
}


void readSection(std::ifstream& infile, bool& binaryFormat, const std::string endSectionKeyword = "") {
	
	if (infile.is_open()) {
		char currC2[2];
		bool reachedSectionEnd(false);
		while (!reachedSectionEnd  && infile.read(currC2, sizeof(currC2)))
		{
			bool keyywordRead(false);
			if (currC2[0] == '$' && currC2[1] == '$') {
				// Start of a keyword, read till a non A-Z char is hit
				std::string keyword;
				char currC;
				while (!reachedSectionEnd && infile.read(&currC, sizeof(currC)) ) {
					if (!isValidCliChar(currC)) {
						continue;
					}
					if (currC == '/') {
						if (infile.peek() == '/') {
							// Start of a comment
							infile.seekg(-1, std::ios::cur);
							skipComment(infile);
						}
					}
					if (isValidKeywordChar(currC))
					{
						keyword += currC;
					}
					if (keyword == "BINARY") {
						binaryFormat = true;
					}
					if (isPastEndCmdChar(currC) || keyword == endSectionKeyword ) {
						std::cout << "Keyword : " << keyword << std::endl;
						keyywordRead = true;
						if (keyword == endSectionKeyword) {
							reachedSectionEnd = true;
						}
						else {
							infile.seekg(-1, std::ios_base::cur);
							break;
						}
						
					}
				}
			}
			else {
				infile.seekg(-1, std::ios_base::cur);
			}

			if (keyywordRead && !reachedSectionEnd) {
				char currC;
				while (!reachedSectionEnd && infile.read(&currC, sizeof(currC))) {
					if (isLineEnd(currC)) {
						break;
					}
					if (currC == '/') {
						// May be start of a parameter block
						if (infile.peek() == '/') {
							infile.seekg(-1, std::ios::cur);
							skipComment(infile);
							continue;
						}
					}
					std::string parameters;
					if (currC == '/') {
						char currC;
						while (!reachedSectionEnd && infile.read(&currC, sizeof(currC))) {
							if (isValidCliChar(currC) && !isLineEnd(currC)) {
								parameters += currC;
							}
							if (isLineEnd(currC)) {
								std::cout << " : Parameter : " << parameters << std::endl;
								infile.seekg(-1, std::ios_base::cur);
								break;
							}
						}
					}

				}
			}

		}
		if (!reachedSectionEnd && !infile.eof()) {
			std::cout << "\n *** Err reading file \n";
		}
	}
}
*/

int main(int argc, char** argv)
{
	CmdLine& cmd = CmdLine::Instance();
	if (!cmd.Initialize(argc, argv)) {
		exit(EXIT_FAILURE);
	}

	std::filesystem::path fileName(cmd.getString("f"));
	fileName.make_preferred();


	CliParser parser(fileName);

	try {
		parser.Initialize();
	}
	catch (...) {
		std::cout << "SomethingWentWrong";
	}
	//std::ifstream infile;
	//infile.open(fileName, std::ios::binary | std::ios::in);

	//infile.open("C:\\Users\\Puja & Kaushik\\Downloads\\VulcanForms-Candidate-Kit-07-17-20\\CLI\\Box-binary.cli", std::ios::binary | std::ios::in);
	//infile.open("C:\\Users\\Puja & Kaushik\\Downloads\\VulcanForms-Candidate-Kit-07-17-20\\CLI\\Box.cli", std::ios::binary | std::ios::in);

	bool binaryFormat(false);
	//readSection(infile, binaryFormat, "HEADEREND");
	parser.ReadASCIISection(binaryFormat, "HEADEREND");

	if (!binaryFormat) {
		//readSection(infile, binaryFormat, "GEOMETRYEND");
		parser.ReadASCIISection(binaryFormat, "GEOMETRYEND");
	}
	else {
		parser.ReadBinarySection();
	}
	
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
