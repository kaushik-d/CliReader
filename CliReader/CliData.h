#pragma once
#include "PolyLine.h"
#include "Hatch.h"

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

	double getCurrentLayerZ() const { return m_layerZ; }
	void setCurrentLayerZ( double z) { m_layerZ = z; }

	std::vector<PolyLine>& Ploylines() { return m_polylines; }
	std::vector<Hatch>& Hatches() { return m_hatches; }

	const std::vector<PolyLine>& getPloylines() { return m_polylines; }
	const std::vector<Hatch>& getHatches() { return m_hatches; }

private:

	Format m_format{ Format::ascii };
	double m_unit{ 1.0 }; 
	double m_layerZ{ 0.0 };

	std::vector<PolyLine> m_polylines;
	std::vector<Hatch> m_hatches;
};

