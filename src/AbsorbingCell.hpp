#ifndef ABSORBINGCELL_HPP
#define ABSORBINGCELL_HPP

#include "Cell.hpp"

class AbsorbingCell:
	public Cell
{
	public:
		AbsorbingCell() = default;

		void setColor(const QColor & color) override;

		void switchColor(const QColor & color) override;

		void setLock(bool lock) override;

		void switchLock() override;
};

#endif // ABSORBINGCELL_HPP
