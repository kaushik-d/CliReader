#pragma once
#include "CliKwyInterpreter.h"

class CliASCIIKywInterpreter : public CliKwyInterpreter
{
public:
	CliASCIIKywInterpreter(std::ifstream& infile) : CliKwyInterpreter(infile) {}
};

