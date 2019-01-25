#include "Lab4Processor.hpp"
#include "SpaceProperties.hpp"
#include "Palette.hpp"
#include "random.hpp"

#include <QHash>
#include <QtDebug>

Lab4Processor::Lab4Processor(QObject * parent):
	Parent(parent),
	m_threshold(INITIAL_THRESHOLD)
{
}

double Lab4Processor::threshold() const
{
	return m_threshold;
}

void Lab4Processor::setThreshold(double threshold)
{
	if (m_threshold != threshold) {
		m_threshold = threshold;
		emit thresholdChanged();
	}
}

bool Lab4Processor::process(Space & active, Space & bg)
{
	const SpaceProperties * props = active.properties();
	for (int row = 0; row < props->rows(); row++)
		for (int col = 0; col < props->columns(); col++) {
			if (active(row, col).hasGrain())
				//  Copy properties from active space to the background.
				bg(row, col) = active(row, col);
			else if (!active(row, col).hasInclusion())
				processCell(active, bg, row, col);
		}
	return true;
}

void Lab4Processor::processCell(Space & active, Space & bg, int row, int col)
{
	QColor newColor = rule1(active, row, col);
	if (newColor == Palette::Instance().empty()) {
		newColor = rule2(active, row, col);
		if (newColor == Palette::Instance().empty()) {
			newColor = rule3(active, row, col);
			if (newColor == Palette::Instance().empty())
				newColor = rule4(active, row, col);
		}
	}
	if (newColor != Palette::Instance().empty())
		bg(row, col).setColor(newColor);
}

QColor Lab4Processor::rule1(Space &active, int row, int col)
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
	colorCounter.remove(QColor(Palette::Instance().inclusion()).rgba64());

	if (colorCounter.count() == 0)
		return QColor(Palette::Instance().empty());

	auto largest = colorCounter.begin();
	for (auto it = colorCounter.begin() + 1; it != colorCounter.end(); ++it)
		if (it.value() > largest.value())
			largest = it;

	if (largest.value() >= 5)
		return QColor(largest.key());
	else
		return Palette::Instance().empty();
}

QColor Lab4Processor::rule2(Space &active, int row, int col)
{
	QHash<QRgba64, int> colorCounter;

	colorCounter[active(row - 1, col).color().rgba64()]++;
	colorCounter[active(row, col + 1).color().rgba64()]++;
	colorCounter[active(row, col - 1).color().rgba64()]++;
	colorCounter[active(row + 1, col).color().rgba64()]++;

	colorCounter.remove(QColor(Palette::Instance().empty()).rgba64());
	colorCounter.remove(QColor(Palette::Instance().inclusion()).rgba64());

	if (colorCounter.count() == 0)
		return QColor(Palette::Instance().empty());

	auto largest = colorCounter.begin();
	for (auto it = colorCounter.begin() + 1; it != colorCounter.end(); ++it)
		if (it.value() > largest.value())
			largest = it;

	if (largest.value() >= 3)
		return QColor(largest.key());
	else
		return Palette::Instance().empty();
}

QColor Lab4Processor::rule3(Space &active, int row, int col)
{
	QHash<QRgba64, int> colorCounter;

	colorCounter[active(row - 1, col - 1).color().rgba64()]++;
	colorCounter[active(row - 1, col + 1).color().rgba64()]++;
	colorCounter[active(row + 1, col - 1).color().rgba64()]++;
	colorCounter[active(row + 1, col + 1).color().rgba64()]++;

	colorCounter.remove(QColor(Palette::Instance().empty()).rgba64());
	colorCounter.remove(QColor(Palette::Instance().inclusion()).rgba64());

	if (colorCounter.count() == 0)
		return QColor(Palette::Instance().empty());

	auto largest = colorCounter.begin();
	for (auto it = colorCounter.begin() + 1; it != colorCounter.end(); ++it)
		if (it.value() > largest.value())
			largest = it;

	if (largest.value() >= 3)
		return QColor(largest.key());
	else
		return Palette::Instance().empty();
}

QColor Lab4Processor::rule4(Space & active, int row, int col)
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
	colorCounter.remove(QColor(Palette::Instance().inclusion()).rgba64());

	if (colorCounter.count() == 0)
		return QColor(Palette::Instance().empty());

	auto largest = colorCounter.begin();
	for (auto it = colorCounter.begin() + 1; it != colorCounter.end(); ++it)
		if (it.value() > largest.value())
			largest = it;

	if (brandom(threshold()))
		return QColor(largest.key());
	else
		return Palette::Instance().empty();
}
