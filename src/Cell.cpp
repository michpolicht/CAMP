#include "Cell.hpp"
#include "SpaceProperties.hpp"
#include "Palette.hpp"

#include <QtDebug>

Cell::Cell():
	m_color(Palette::Instance().empty()),
	m_lockColor(Palette::Instance().empty())
{
}

bool Cell::hasGrain() const
{
	return m_color.rgba64() != Palette::Instance().empty().rgba64()
			&& m_color.rgba64() != Palette::Instance().inclusion().rgba64();
}

bool Cell::hasInclusion() const
{
	return m_color.rgba64() == Palette::Instance().inclusion().rgba64();
}

bool Cell::isActive() const
{
	return hasGrain() && !isLocked();
}

bool Cell::isEmpty() const
{
	return m_color.rgba64() == Palette::Instance().empty().rgba64();
}

bool Cell::isFilled() const
{
	return m_color.rgba64() != Palette::Instance().empty().rgba64();
}

bool Cell::isLocked() const
{
	return m_lockColor.rgba64() != Palette::Instance().empty().rgba64();
}

QColor Cell::color() const
{
	return m_color;
}

QColor Cell::realColor() const
{
	if (isLocked())
		return m_lockColor;
	else
		return m_color;
}

void Cell::setColor(const QColor & color)
{
	m_color = color;
}

void Cell::setRealColor(const QColor & color)
{
	if (isLocked())
		m_lockColor = color;
	else
		m_color = color;
}

void Cell::switchColor(const QColor & color)
{
	if (m_color.rgba64() == color.rgba64())
		m_color = Palette::Instance().empty();
	else
		m_color = color;
}

void Cell::setLock(bool lock)
{
	if (lock != isLocked()) {
		if (lock) {
			// Store original color.
			m_lockColor = m_color;
			m_color = Palette::Instance().inclusion();
		} else {
			// Restore color.
			m_color = m_lockColor;
			m_lockColor = Palette::Instance().empty();
		}
	}
}

void Cell::switchLock()
{
	setLock(!isLocked());
}

void Cell::reset()
{
	setColor(Palette::Instance().empty());
	setLock(false);
}

void Cell::resetSoft()
{
	setColor(Palette::Instance().empty());
}
