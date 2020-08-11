#pragma once
#include <map>
#include <array>
#include <limits>
#include <filesystem>

#include "PolyLine.h"
#include "Hatch.h"

enum class Format {
	ascii = 0,
	binary = 1,
};

class CliData
{
public:
	CliData();
	virtual ~CliData() {};

	CliData(const CliData& c) = delete;
	CliData& operator=(const CliData& c) = delete;

	Format getFormat() const { return m_format; }
	void setFormat(Format format) { m_format = format; }

	double getUnit() const { return m_unit; }
	void setUnit(double unit) { m_unit = unit; }
	void setVersion(int ver) { m_version = ver; }

	double getCurrentLayerZ() const { return m_layerZ; }
	void setCurrentLayerZ(double z) { m_layerZ = z; }

	std::vector<PolyLine>& Ploylines() { return m_polylines; }
	std::vector<Hatch>& Hatches() { return m_hatches; }

	const std::vector<PolyLine>& getPloylines() { return m_polylines; }
	const std::vector<Hatch>& getHatches() { return m_hatches; }

	void set32bitAlign() { m_align32bit = true; }
	void unset32bitAlign() { m_align32bit = false; }
	bool is32bitAlign() { return m_align32bit; }

	void incrmentLayerIndex() { m_layerIndex++; }
	int getLayerIndex() { return m_layerIndex; }
	std::array<double, 6>& Dimension() { return m_dimension; }

	void clearLayer();
	void layerArea();
	void printLayer();

	void printLayerInSVG();

private:

	// Information from file

	Format m_format{ Format::ascii };
	int m_version{ 0 };
	double m_unit{ 1.0 };

	bool m_align32bit{ false };

	int m_layerIndex{ -1 };
	double m_layerZ{ -1.0 }; // Initializing with a out of bound value

	std::vector<PolyLine> m_polylines;
	std::vector<Hatch> m_hatches;
	std::array<double, 6> m_dimension
	{
		std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max(),
		std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest()
	};

	std::array<double, 4> m_dimFromLayer
	{
		std::numeric_limits<double>::max(),std::numeric_limits<double>::max(),
		std::numeric_limits<double>::lowest(),std::numeric_limits<double>::lowest()
	};

	// Information drived from file data
	double m_layer_area{ -1 };
	std::map<int, double> m_partArea;

	// html file
	bool m_writeHTML{ false };
	std::filesystem::path m_path;
	std::filesystem::path m_cliFileName;
};

