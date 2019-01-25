#include <QtDebug>

#include "BoundaryColoringProcessor.hpp"
#include "SpaceProperties.hpp"
#include "Palette.hpp"

BoundaryColoringProcessor::BoundaryColoringProcessor(QObject * parent):
	Parent(parent),
	m_width(1)
{
}

int BoundaryColoringProcessor::width() const
{
	return m_width;
}

void BoundaryColoringProcessor::setWidth(int width)
{
	if (m_width != width) {
		m_width = width;
		emit widthChanged();
	}
}

bool BoundaryColoringProcessor::process(Space & active, Space & bg)
{
	qDebug() << "BoundaryColoringProcessor starts processing space...";


	const SpaceProperties * props = active.properties();

	for (int col = 0; col < props->columns(); col++)
		for (int row = 0; row < props->rows(); row++) {
			if (active(row, col).isLocked())
				active(row, col).switchLock();
			if (bg(row, col).isLocked())
				bg(row, col).switchLock();
		}

	for (int row = 0; row < props->rows(); row++) {
		QColor prevColor = Palette::Instance().inclusion();
//		QColor prevColor = active(row, -1).color();
		for (int col = 0; col < props->columns(); col++) {
			if (active(row, col).realColor().rgba64() != prevColor.rgba64()
				&& prevColor.rgba64() != Palette::Instance().inclusion().rgba64()) {
				prevColor = active(row, col).color();
				for (int w = 0; w < m_width; w++) {
					active(row, col + w).setColor(Palette::Instance().inclusion());
					bg(row, col + w).setColor(Palette::Instance().inclusion());
				}
			} else
				prevColor = active(row, col).realColor();
		}
	}

	for (int col = 0; col < props->columns(); col++) {
//		QColor prevColor = active(-1, col).color();
		QColor prevColor = Palette::Instance().inclusion();
		for (int row = 0; row < props->rows(); row++) {
			if (active(row, col).realColor().rgba64() != prevColor.rgba64()
				&& prevColor.rgba64() != Palette::Instance().inclusion().rgba64()) {
				prevColor = active(row, col).color();
				for (int w = 0; w < m_width; w++) {
					active(row + w, col).setColor(Palette::Instance().inclusion());
					bg(row + w, col).setColor(Palette::Instance().inclusion());
				}
			} else
				prevColor = active(row, col).color();
		}
	}

	return true;
}
