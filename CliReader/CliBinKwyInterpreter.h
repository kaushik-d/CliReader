#pragma once
#include "CliASCIIKywInterpreter.h"

class CliBinKwyInterpreter : public CliASCIIKywInterpreter
{
public:

	CliBinKwyInterpreter(std::ifstream& infile) : CliASCIIKywInterpreter(infile) {}
	virtual ~CliBinKwyInterpreter() {}

	virtual void ParseStartLayerLong();
	virtual void ParseStartLayerShort();
	virtual void ParseStartPolyLineShort();
	virtual void ParseStartPolyLineLong();
	virtual void ParseStartHatchesShort();
	virtual void ParseStartHatchesLong();

	CliBinKwyInterpreter(const CliBinKwyInterpreter& obj) = delete;
	CliBinKwyInterpreter& operator = (const CliBinKwyInterpreter& obj) = delete;

};

