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
	void ParseStartLayer();

	template<class Pram_t, int Param_N, class Data_t, int Id, int Dir, int Npt>
	void ParseStartPolyLine();

	template<class Pram_t, int Param_N, class Data_t, int Id, int Npt>
	void ParseStartHatches();

	void InterpretBinKwy(BinKeyword kwy);

};

