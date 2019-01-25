#ifndef SPACE_HPP
#define SPACE_HPP

#include "Cell.hpp"
#include "DoubleBuffer.hpp"

#include <QVector>
#include <QObject>

class SpaceProperties;

class Space
{
	Q_GADGET

	public:
		Space(SpaceProperties * properties);

		Space(const Space & other);

		virtual ~Space();

		Space & operator =(const Space & other);

		const Cell & operator()(int row, int col) const;

		Cell & operator()(int row, int col);

//		const Cell & topLeft(int row, int col) const;

//		Cell & topLeft(int row, int col);

//		const Cell & top(int row, int col) const;

//		Cell & top(int row, int col);

//		const Cell & topRight(int row, int col) const;

//		Cell & topRight(int row, int col);

//		const Cell & left(int row, int col) const;

//		Cell & left(int row, int col);

//		const Cell & right(int row, int col) const;

//		Cell & right(int row, int col);

//		const Cell & bottomLeft(int row, int col) const;

//		Cell & bottomLeft(int row, int col);

//		const Cell & bottom(int row, int col) const;

//		Cell & bottom(int row, int col);

//		const Cell & bottomRight(int row, int col) const;

//		Cell & bottomRight(int row, int col);

		const SpaceProperties * properties() const;

		void resize();

		void reset();

		void resetSoft();

		void unifyLocked();

		bool isFilled() const;

		bool isEmpty() const;

		bool hasGrains() const;

		bool toggleGrainLock(int row, int column);

    private:
		typedef QVector<Cell> CellsContainer;

		int mod(int a, int b) const;

		SpaceProperties * m_properties;
		CellsContainer m_cells;
};

#endif // CASPACE_HPP
