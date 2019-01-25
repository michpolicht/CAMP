#include "InclusionsProcessor.hpp"
#include "SpaceProperties.hpp"
#include "Palette.hpp"
#include "random.hpp"

#include <QtDebug>

InclusionsProcessor::InclusionsProcessor(QObject * parent):
	Parent(parent)
{
}

bool InclusionsProcessor::process(Space & active, Space & bg)
{
	qDebug() << "InclusionsProcessor starts processing space...";

	int i = 0;
	int failures = 0;
	const SpaceProperties * props = active.properties();
	while (i < inclusions()) {
		int row = random(0, props->rows() - 1);
		int column = random(0, props->columns() - 1);
		int radius = random(minInclusionRadius(), maxInclusionRadius());

		if (hasInclusion(row, column, radius, active))
			failures++;
		else {
			createInclusion(row, column, radius, active);
			createInclusion(row, column, radius, bg);
			i++;
		}

		if (failures > failedInclusionsLimit()) {
			qWarning() << "Limit of failed inlcusion injections (" << failedInclusionsLimit() << ") exceeded... Aborting.";
			return false;
		}

	}
	return true;
}
