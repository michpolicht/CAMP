#include "MooreProcessor.hpp"
#include "SpaceProperties.hpp"
#include "Palette.hpp"

#include <QHash>
#include <QtDebug>

#define MOORE_PROCESSOR_PROCESS_BASIC

MooreProcessor::MooreProcessor(QObject * parent):
	Parent(parent)
{
}

bool MooreProcessor::process(Space & active, Space & bg)
{
	const SpaceProperties * props = active.properties();
	for (int row = 0; row < props->rows(); row++)
		for (int col = 0; col < props->columns(); col++) {
#ifdef MOORE_PROCESSOR_PROCESS_STRAIGHT
			if (!active(row, col).hasGrain())
				continue;

			// Copy color from active space to the background.
			bg(row, col).setColor(active(row, col).color());

			processCell(active, bg, row - 1, col - 1);
			processCell(active, bg, row - 1, col);
			processCell(active, bg, row - 1, col + 1);
			processCell(active, bg, row, col - 1);
			processCell(active, bg, row, col + 1);
			processCell(active, bg, row + 1, col - 1);
			processCell(active, bg, row + 1, col);
			processCell(active, bg, row + 1, col + 1);
#endif
#ifdef MOORE_PROCESSOR_PROCESS_BASIC
			if (active(row, col).hasGrain())
				//  Copy color from active space to the background.
				bg(row, col).setColor(active(row, col).color());
			else
				processCell(active, bg, row, col);
#endif
		}
	return true;
}

void MooreProcessor::processCell(Space & active, Space & bg, int row, int col)
{
	if (!active(row, col).hasGrain()) {
		QColor newColor = findDominantNeighbour(active, row, col);
		bg(row, col).setColor(newColor);
	}
}

QColor MooreProcessor::findDominantNeighbour(Space & active, int row, int col)
{
	QHash<QRgba64, int> colorCounter;

	colorCounter[active(row - 1, col - 1).color().rgba64()]++;
	colorCounter[active(row - 1, col).color().rgba64()]++;
	colorCounter[active(row - 1, col + 1).color().rgba64()]++;
	colorCounter[active(row, col + 1).color().rgba64()]++;
	colorCounter[active(row, col - 1).color().rgba64()]++;
	colorCounter[active(row + 1, col - 1).color().rgba64()]++;
	colorCounter[active(row + 1, col).color().rgba64()]++;
	colorCounter[active(row + 1, col + 1).color().rgba64()]++;

	colorCounter.remove(QColor(Palette::Instance().empty()).rgba64());

	if (colorCounter.count() == 0)
		return QColor(Palette::Instance().empty());

	auto largest = colorCounter.begin();
	for (auto it = colorCounter.begin() + 1; it != colorCounter.end(); ++it)
		if (it.value() > largest.value())
			largest = it;

	return QColor(largest.key());
}
