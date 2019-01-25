#include "BoundaryInclusionsProcessor.hpp"
#include "SpaceProperties.hpp"
#include "Palette.hpp"
#include "random.hpp"

#include <QtDebug>

BoundaryInclusionsProcessor::BoundaryInclusionsProcessor(QObject * parent):
	Parent(parent)
{
}

bool BoundaryInclusionsProcessor::process(Space & active, Space & bg)
{
	qDebug() << "BoundaryInclusionsProcessor starts processing space...";

	int failures = 0;
	int i = 0;
	const SpaceProperties * props = active.properties();
	while (i < inclusions()) {
		int row = random(0, props->rows() - 1);
		int column = random(0, props->columns() - 1);
		int radius = random(minInclusionRadius(), maxInclusionRadius());

		std::pair<int, int> newPos = findBoundaryPos(row, column, active);
		if (newPos.first == -1 || hasInclusion(newPos.first, newPos.second, radius, active))
			failures++;
		else {
			createInclusion(newPos.first, newPos.second, radius, bg);
			createInclusion(newPos.first, newPos.second, radius, active);
			i++;
		}

		if (failures > failedInclusionsLimit()) {
			qWarning() << "Limit of failed inlcusion injections (" << failedInclusionsLimit() << ") exceeded... Aborting.";
			return false;
		}
	}
	return true;
}

std::pair<int, int> BoundaryInclusionsProcessor::findBoundaryPos(int row, int col, const Space & space) const
{
	int rowMove = 0;
	int columnMove = 0;
//	 For simplicity perform either horizontal or vertical movement.
	if (brandom())
		rowMove = brandom() ? 1 : -1;
	else
		columnMove = brandom() ? 1 : -1;

	std::pair<int, int> newPos(row + rowMove, col + columnMove);
	while (newPos.first >= 0
		   && newPos.first < space.properties()->rows()
		   && newPos.second >= 0
		   && newPos.second < space.properties()->columns()
		   ) {
		if (space(newPos.first, newPos.second).color().rgba64() != Palette::Instance().inclusion().rgba64()
			&& space(row, col).color().rgba64() != Palette::Instance().inclusion().rgba64()
			&& space(newPos.first, newPos.second).color().rgba64() != space(row, col).color().rgba64()) {
				return newPos;
		} else {
			row = newPos.first;
			col = newPos.second;
			newPos.first += rowMove;
			newPos.second += columnMove;
		}
	}
	return std::pair<int, int>(-1, -1);
}
