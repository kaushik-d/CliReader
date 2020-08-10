#pragma once
#include "PolyLine.h"
#include "Hatch.h"
#include <map>

enum class Format {
	ascii = 0,
	binary = 1,
};

class CliData
{
public:
	Format getFormat() const { return m_format; }
	void setFormat(Format format) { m_format = format; }

	double getUnit() const { return m_unit; }
	void setUnit(double unit) { m_unit = unit; }
	void setVersion(int ver) { m_version = ver; }

	double getCurrentLayerZ() const { return m_layerZ; }
	void setCurrentLayerZ( double z) { m_layerZ = z; }

	std::vector<PolyLine>& Ploylines() { return m_polylines; }
	std::vector<Hatch>& Hatches() { return m_hatches; }

	const std::vector<PolyLine>& getPloylines() { return m_polylines; }
	const std::vector<Hatch>& getHatches() { return m_hatches; }

	void set32bitAlign() { m_align32bit = true; }
	void unset32bitAlign() { m_align32bit = false; }
	bool is32bitAlign() { return m_align32bit; }

	void incrmentLayerIndex() { m_layerIndex++; }
	int getLayerIndex() { return m_layerIndex; }

	void clearLayer();
	void layerArea();
	void printLayer();

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

	// Information drived from file data

	double m_layer_area{ -1 };
	std::map<int, double> m_partArea;
};

