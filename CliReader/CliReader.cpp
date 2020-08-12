// CliReader.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>

#include "CmdLine.h"
#include "CliParser.h"
#include "CliData.h"
#include "CliHtmlWriter.h"

int main(int argc, char** argv)
{
	try {
		CmdLine& cmd = CmdLine::Instance();
		if (!cmd.Initialize(argc, argv)) {
			exit(EXIT_FAILURE);
		}

		if(cmd.isDefined("html")) {
			CliHtmlWriter::Instance().Inialize();
		}

		CliData cliData;

		std::filesystem::path fileName(cmd.getString("f"));
		fileName.make_preferred();
		CliParser parser(fileName, cliData);

		parser.Initialize();
		parser.ParseFile();

		if (cmd.isDefined("html")) {
			CliHtmlWriter::Instance().CliWriteHtmlFile();
		}
	}
	catch (std::runtime_error& e) {
		std::cout << "Error : " << e.what() << std::endl;
	}
	catch (...) {
		std::cout << "Error : SYSTEM ERROR" << std::endl ;
	}

}
