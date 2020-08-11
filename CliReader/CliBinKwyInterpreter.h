#pragma once
#include "CliASCIIKywInterpreter.h"
#include "CliKwyLib.h"

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

	template<class T, int N>
	void ParseStartLayerShort();

	template<class Pram_t, int Param_N, class Data_t, int Id, int Dir, int Npt>
	void ParseStartPolyLineShort();
	void ParseStartPolyLineLong();

	template<class Pram_t, int Param_N, class Data_t, int Id, int Npt>
	void ParseStartHatchesShort();
	void ParseStartHatchesLong();
	void InterpretBinKwy(BinKeyword kwy);

};

