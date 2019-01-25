#include "AbstractInclusionsProcessor.hpp"
#include "Palette.hpp"

AbstractInclusionsProcessor::AbstractInclusionsProcessor(QObject * parent):
	Parent(parent),
	m_inclusions(INITIAL_INCLUSIONS),
	m_minInclusionRadius(INITIAL_MIN_INCLUSION_RADIUS),
	m_maxInclusionRadius(INITIAL_MAX_INCLUSION_RADIUS),
	m_failedInclusionsLimit(INITIAL_FAILED_INCLUSIONS_LIMIT)
{
}

int AbstractInclusionsProcessor::inclusions() const
{
	return m_inclusions;
}

void AbstractInclusionsProcessor::setInclusions(int inclusions)
{
	if (m_inclusions != inclusions) {
		m_inclusions = inclusions;
		emit inclusionsChanged();
	}
}

int AbstractInclusionsProcessor::minInclusionRadius() const
{
	return m_minInclusionRadius;
}

void AbstractInclusionsProcessor::setMinInclusionRadius(int radius)
{
	if (m_minInclusionRadius != radius) {
		m_minInclusionRadius = radius;
		emit minInclusionRadiusChanged();
	}
}

int AbstractInclusionsProcessor::maxInclusionRadius() const
{
	return m_maxInclusionRadius;
}

void AbstractInclusionsProcessor::setMaxInclusionRadius(int radius)
{
	if (m_maxInclusionRadius != radius) {
		m_maxInclusionRadius = radius;
		emit maxInclusionRadiusChanged();
	}
}

int AbstractInclusionsProcessor::failedInclusionsLimit() const
{
	return m_failedInclusionsLimit;
}

void AbstractInclusionsProcessor::setFailedInclusionsLimit(int limit)
{
	if (m_failedInclusionsLimit != limit) {
		m_failedInclusionsLimit = limit;
		emit failedInclusionsLimitChanged();
	}
}

void AbstractInclusionsProcessor::createInclusion(int row, int col, int radius, Space & space)
{
//	int x = radius - 1;
//	int y = 0;
//	int dx = 1;
//	int dy = 1;
//	int err = dx - (radius << 1);
//	int y0 = row;
//	int x0 = col;
//	while (x >= y)
//	{
//		space(x0 + x, y0 + y).setColor(Palette::Instance().inclusion());
//		space(x0 + y, y0 + x).setColor(Palette::Instance().inclusion());
//		space(x0 - y, y0 + x).setColor(Palette::Instance().inclusion());
//		space(x0 - x, y0 + y).setColor(Palette::Instance().inclusion());
//		space(x0 - x, y0 - y).setColor(Palette::Instance().inclusion());
//		space(x0 - y, y0 - x).setColor(Palette::Instance().inclusion());
//		space(x0 + y, y0 - x).setColor(Palette::Instance().inclusion());
//		space(x0 + x, y0 - y).setColor(Palette::Instance().inclusion());

//		if (err <= 0)
//		{
//			y++;
//			err += dy;
//			dy += 2;
//		}

//		if (err > 0)
//		{
//			x--;
//			dx += 2;
//			err += dx - (radius << 1);
//		}
//	}

	for (int dy = 0; dy < radius; dy++)
		for (int dx = 0; dx * dx < radius * radius - dy * dy; dx++) {
			space(row + dy, col + dx).setColor(Palette::Instance().inclusion());
			space(row + dy, col - dx).setColor(Palette::Instance().inclusion());
			space(row - dy, col + dx).setColor(Palette::Instance().inclusion());
			space(row - dy, col - dx).setColor(Palette::Instance().inclusion());
		}
}

bool AbstractInclusionsProcessor::hasInclusion(int row, int col, int radius, const Space & space) const
{
	for (int dy = 0; dy < radius; dy++)
		for (int dx = 0; dx * dx < radius * radius - dy * dy; dx++) {
			if (space(row + dy, col + dx).hasInclusion())
				return true;
			if (space(row + dy, col - dx).hasInclusion())
				return true;
			if (space(row - dy, col + dx).hasInclusion())
				return true;
			if (space(row - dy, col - dx).hasInclusion())
				return true;
		}
	return false;
}
