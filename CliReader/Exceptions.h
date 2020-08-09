#pragma once
#include <stdexcept>

class CliReaderErr : public std::runtime_error
{
public:
	CliReaderErr(const std::string& what_arg) : std::runtime_error(what_arg) {
	}
};

class CmdInvalidOptions : public CliReaderErr
{
public:
	CmdInvalidOptions(const std::string& what_arg) : CliReaderErr(what_arg) {
	}
};

class FileNotFound : public CliReaderErr
{
public:
	FileNotFound(const std::string& what_arg) : CliReaderErr(what_arg) {
	}
};
