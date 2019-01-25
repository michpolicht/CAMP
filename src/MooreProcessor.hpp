#ifndef MOOREPROCESSOR_HPP
#define MOOREPROCESSOR_HPP

#include "AbstractSpaceProcessor.hpp"

class MooreProcessor:
	public AbstractSpaceProcessor
{
	Q_OBJECT

	typedef AbstractSpaceProcessor Parent;

	public:
		MooreProcessor(QObject * parent = nullptr);

		bool process(Space & active, Space & bg) override;

	private:
		void processCell(Space & active, Space & bg, int row, int col);

		QColor findDominantNeighbour(Space & active, int row, int col);
};

#endif // MOOREPROCESSOR_HPP
