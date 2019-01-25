#include "Space.hpp"
#include "SpaceProperties.hpp"
#include "AbsorbingCell.hpp"

#include <QtDebug>

Space::Space(SpaceProperties * properties):
	m_properties(properties)
{
	m_properties->addObserver(this);
}

Space::Space(const Space & other):
	m_properties(other.m_properties),
	m_cells(other.m_cells)
{
	m_properties->addObserver(this);
}

Space::~Space()
{
	m_properties->removeObserver(this);
}

Space & Space::operator =(const Space & other)
{
	// Handle self-assignment.
	if (this == & other)
		return *this;

	m_properties = other.m_properties;
	m_properties->addObserver(this);	// Without self-assignment branch object could be added to observers multiple times.
	m_cells = other.m_cells;
	return *this;
}

const Cell & Space::operator()(int row, int col) const
{
	return const_cast<Space *>(this)->operator()(row, col);
}

Cell & Space::operator()(int row, int col)
{
	static AbsorbingCell absorbingCell;

	// Helper variables.
	SpaceProperties::BoundaryType boundaryType = m_properties->boundaryType();
	int rows = m_properties->rows();
	int columns = m_properties->columns();

	if (boundaryType == SpaceProperties::ABSORBING) {
		if (col < 0 || row < 0 || col >= columns || row >= rows)
			return absorbingCell;
		return m_cells[col * rows + row];
	} else if (boundaryType == SpaceProperties::PERIODIC) {
		return m_cells[(mod(col, columns)) * rows + (mod(row, rows))];
	} else {
		qWarning() << "Invalid space type:" << boundaryType;
		return absorbingCell;
	}
}

//const Cell & Space::topLeft(int row, int col) const
//{
//	return const_cast<Space *>(this)->topLeft(row, col);
//}

//Cell & Space::topLeft(int row, int col)
//{
//	return (*this)(row - 1, col - 1);
//}

//const Cell & Space::top(int row, int col) const
//{
//	return const_cast<Space *>(this)->top(row, col);
//}

//Cell & Space::top(int row, int col)
//{
//	return (*this)(row - 1, col);
//}

//const Cell & Space::topRight(int row, int col) const
//{
//	return const_cast<Space *>(this)->topRight(row, col);
//}

//Cell &Space::topRight(int row, int col)
//{
//	return (*this)(row - 1, col + 1);
//}

//const Cell & Space::left(int row, int col) const
//{
//	return const_cast<Space *>(this)->left(row, col);
//}

//Cell & Space::left(int row, int col)
//{
//	return (*this)(row, col - 1);
//}

//const Cell & Space::right(int row, int col) const
//{
//	return const_cast<Space *>(this)->right(row, col);
//}

//Cell & Space::right(int row, int col)
//{
//	return (*this)(row, col + 1);
//}

//const Cell & Space::bottomLeft(int row, int col) const
//{
//	return const_cast<Space *>(this)->bottomLeft(row, col);
//}

//Cell & Space::bottomLeft(int row, int col)
//{
//	return (*this)(row + 1, col - 1);
//}

//const Cell & Space::bottom(int row, int col) const
//{
//	return const_cast<Space *>(this)->bottom(row, col);
//}

//Cell & Space::bottom(int row, int col)
//{
//	return (*this)(row + 1, col);
//}

//const Cell & Space::bottomRight(int row, int col) const
//{
//	return const_cast<Space *>(this)->bottomRight(row, col);
//}

//Cell & Space::bottomRight(int row, int col)
//{
//	return (*this)(row + 1, col + 1);
//}

const SpaceProperties * Space::properties() const
{
	return m_properties;
}

void Space::resize()
{
	m_cells.resize(m_properties->rows() * m_properties->columns());
}

void Space::reset()
{
	for (CellsContainer::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
		it->reset();
}

void Space::resetSoft()
{
	for (CellsContainer::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
		if (!it->isLocked() && !it->hasInclusion())
			it->resetSoft();
}

void Space::unifyLocked()
{
	QColor unifyColor;
	for (CellsContainer::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
		if (it->isLocked())
			unifyColor = it->realColor();
	if (unifyColor.isValid())
		for (CellsContainer::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
			if (it->isLocked()) {
				it->switchLock();
				it->setColor(unifyColor);
				it->switchLock();
			}
}

bool Space::isFilled() const
{
	for (CellsContainer::const_iterator it = m_cells.begin(); it != m_cells.end(); ++it)
		if (!it->isFilled())
			return false;
	return true;
}

bool Space::isEmpty() const
{
	for (CellsContainer::const_iterator it = m_cells.begin(); it != m_cells.end(); ++it)
		if (!it->isEmpty())
			return false;
	return true;
}

bool Space::hasGrains() const
{
	for (CellsContainer::const_iterator it = m_cells.begin(); it != m_cells.end(); ++it)
		if (it->hasGrain())
			return true;
	return false;
}

bool Space::toggleGrainLock(int row, int column)
{
	if (!(*this)(row, column).hasGrain() && !(*this)(row, column).isLocked())
		return false;
	QRgba64 color = (*this)(row, column).realColor().rgba64();
	for (CellsContainer::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
		if (it->realColor().rgba64() == color)
			it->switchLock();
	return (*this)(row, column).isLocked();
}

int Space::mod(int a, int b) const
{
	int result = a % b;
	if (result < 0) {
		result = (b < 0) ? result - b : result + b;
	}
	return result;
}
