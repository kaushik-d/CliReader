#pragma once
#include <filesystem>
#include <vector>
#include <map>
#include <array>
#include "PolyLine.h"

class CliHtmlWriter
{
//
// This is a singlenton class
//
public:
	static CliHtmlWriter& Instance()
	{
		static CliHtmlWriter instance;
		return instance;
	}

	void Inialize();
	void addLayer(int layerId, double area, const std::vector<PolyLine>& polylines);

	void CliWriteHtmlFile();

private:
	CliHtmlWriter() {}
	bool initialized{ false };

	std::filesystem::path m_path;
	std::filesystem::path m_cliFileName;

	std::map<int,std::vector<PolyLine>> m_polylines;
	std::vector<double> m_area;

	std::array<double, 4> m_dimFromLayer
	{
		std::numeric_limits<double>::max(),std::numeric_limits<double>::max(),
		std::numeric_limits<double>::lowest(),std::numeric_limits<double>::lowest()
	};

public:
	CliHtmlWriter(CliHtmlWriter const&) = delete;
	void operator=(CliHtmlWriter const&) = delete;

};

