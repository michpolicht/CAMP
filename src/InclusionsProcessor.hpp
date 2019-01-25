#ifndef INCLUSIONSPROCESSOR_HPP
#define INCLUSIONSPROCESSOR_HPP

#include "AbstractInclusionsProcessor.hpp"

class InclusionsProcessor:
		public AbstractInclusionsProcessor
{
	Q_OBJECT

	typedef AbstractInclusionsProcessor Parent;

	public:
		InclusionsProcessor(QObject * parent = nullptr);

		bool process(Space & active, Space & bg) override;

	private:
//		void processCell(Space & active, Space & bg, int row, int col);

//		QColor findDominantNeighbour(Space & active, int row, int col);
};

#endif // INCLUSIONSPROCESSOR_HPP
