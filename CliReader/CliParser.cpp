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

void CliParser::Initialize()
{
	m_infile.open(m_fileName, std::ios::binary | std::ios::in);
	if (!m_infile.is_open()) {
		throw FileNotFound(m_fileName.string() + " file not found.");
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
