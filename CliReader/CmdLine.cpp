#include <iostream>
#include "CmdLine.h"
#include "Exceptions.h"

bool CmdLine::Initialize(int argc, char** argv) {

	if (argc < 0 || argv == nullptr) {
		return false;
	}
		 
	m_validOptions = {
		{"h", {false, "Display help."} },
		{"f", {true,  "Input CLI file."} }
	};

	bool CmlLineError(false);

	for (int i = 1; i < argc; i++) {
		
		if (argv[i] == nullptr) {
			return false;
		}

		if (argv[i][0] == '-') {

			const char* sOpt = &(argv[i][1]);

			if (sOpt == "" || m_validOptions.find(sOpt) == m_validOptions.end()) {
				std::cout << "\nInvalid command line option " << sOpt << std::endl;
				CmlLineError = true;
			}

			if (i != argc - 1 && argv[i + 1][0] != '-') {

				const char* sVal = argv[i + 1];
				m_opts.insert(std::make_pair(sOpt, sVal));
				i++;
			}
			else {

				std::string val("");
				m_opts.insert(std::make_pair(sOpt, val));
			}
		}
	}

	// Make sure all required options are present on cmdline

	for (auto iter : m_validOptions) {
		if (iter.second.required) {
			if (!isDefined(iter.first)) {
				std::cout << "\nCommand line option " << iter.first << " is required." << std::endl;
				CmlLineError = true;
			}
		}
	}

	if (CmlLineError) {
		return false;
	}

	return true;
}

bool CmdLine::isDefined(const std::string& opt) const
{
	return m_opts.find(opt) != m_opts.end();
}

std::string CmdLine::getString(const std::string& opt) const
{
	auto iter = m_opts.find(opt);
	if (iter == m_opts.end()) {
		throw CmdInvalidOptions(opt);
	}
	else {
		return iter->second.c_str();
	}
}

int CmdLine::getInt(const std::string& opt) const
{
	auto iter = m_opts.find(opt);
	if (iter != m_opts.end()) {
		return 0;
	}
	else {
		return std::atoi(iter->second.c_str());
	}
}

double CmdLine::getDouble(const std::string& opt) const
{
	auto iter = m_opts.find(opt);
	if (iter != m_opts.end()) {
		return 0;
	}
	else {
		return std::atof(iter->second.c_str());
	}
}

void CmdLine::displayHelp()
{
	std::cout << "\n CLI file reader." << std::endl;
	for (auto& opts : m_validOptions) {
		std::cout << opts.first << " : " << (opts.second.required ? " (Required) " : " (Options) ") << " : " << opts.second.help << std::endl;
	}
	std::cout << std::endl;
}
