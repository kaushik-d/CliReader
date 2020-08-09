#pragma once
#include "CliASCIIKywInterpreter.h"

class CliBinKwyInterpreter : public CliASCIIKywInterpreter
{
public:

	CliBinKwyInterpreter(std::ifstream& infile, CliData& cliData) 
		: CliASCIIKywInterpreter(infile, cliData) {}
	virtual ~CliBinKwyInterpreter() {}

	virtual void InterpretHeader();
	virtual void InterpretGeometry();

	CliBinKwyInterpreter(const CliBinKwyInterpreter& obj) = delete;
	CliBinKwyInterpreter& operator = (const CliBinKwyInterpreter& obj) = delete;

private:
	void ParseStartLayerLong();
	void ParseStartLayerShort();
	void ParseStartPolyLineShort();
	void ParseStartPolyLineLong();
	void ParseStartHatchesShort();
	void ParseStartHatchesLong();

};

