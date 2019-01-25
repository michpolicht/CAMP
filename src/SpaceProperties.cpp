#include "SpaceProperties.hpp"
#include "Space.hpp"

SpaceProperties::SpaceProperties(int rows, int columns, SpaceProperties::BoundaryType boundaryType):
	m_rows(rows),
	m_columns(columns),
	m_boundaryType(boundaryType)
{
	Q_ASSERT(rows >= 0);
	Q_ASSERT(columns >= 0);
}

int SpaceProperties::rows() const
{
	return m_rows;
}

void SpaceProperties::setRows(int rows)
{
	Q_ASSERT(rows >= 0);

	m_rows = rows;

	for (ObserversContainer::iterator it = m_observers.begin(); it != m_observers.end(); ++it)
		(*it)->resize();
}

int SpaceProperties::columns() const
{
	return m_columns;
}

void SpaceProperties::setColumns(int columns)
{
	Q_ASSERT(columns >= 0);

	m_columns = columns;

	for (ObserversContainer::iterator it = m_observers.begin(); it != m_observers.end(); ++it)
		(*it)->resize();
}

int SpaceProperties::size() const
{
	return m_rows * m_columns;
}

void SpaceProperties::setSize(int rows, int columns)
{
	m_rows = rows;
	m_columns = columns;

	for (ObserversContainer::iterator it = m_observers.begin(); it != m_observers.end(); ++it)
		(*it)->resize();
}

SpaceProperties::BoundaryType SpaceProperties::boundaryType() const
{
	return m_boundaryType;
}

void SpaceProperties::setBoundaryType(SpaceProperties::BoundaryType boundaryType)
{
	m_boundaryType = boundaryType;
}

void SpaceProperties::addObserver(Space * space)
{
	space->resize();

	m_observers.append(space);
}

void SpaceProperties::removeObserver(Space * space)
{
	m_observers.removeAll(space);
}
