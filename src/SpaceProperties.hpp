#ifndef SPACEPROPERTIES_HPP
#define SPACEPROPERTIES_HPP

#include "Space.hpp"

#include <QList>

class Space;

class SpaceProperties
{
	Q_GADGET

	public:
		enum BoundaryType: int {
			ABSORBING,
			PERIODIC
		};
		Q_ENUM(BoundaryType)

		static constexpr int INITIAL_ROWS = 50;
		static constexpr int INITIAL_COLUMNS = 50;
		static constexpr BoundaryType INITIAL_BOUNDARY_TYPE = ABSORBING;

		SpaceProperties(int rows = INITIAL_ROWS, int columns = INITIAL_COLUMNS, BoundaryType boundaryType = INITIAL_BOUNDARY_TYPE);

		int rows() const;

		void setRows(int rows);

		int columns() const;

		void setColumns(int columns);

		int size() const;

		void setSize(int rows, int columns);

		BoundaryType boundaryType() const;

		void setBoundaryType(BoundaryType boundaryType);

		void addObserver(Space * space);

		void removeObserver(Space * space);

	private:
		typedef QList<Space *> ObserversContainer;

		int m_rows;
		int m_columns;
		BoundaryType m_boundaryType;
		ObserversContainer m_observers;
};

#endif // SPACEPROPERTIES_HPP
