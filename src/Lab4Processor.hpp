#ifndef LAB4PROCESSOR_HPP
#define LAB4PROCESSOR_HPP

#include "AbstractSpaceProcessor.hpp"

#include <QHash>

class Lab4Processor:
	public AbstractSpaceProcessor
{
	Q_OBJECT

	typedef AbstractSpaceProcessor Parent;

	public:
		Q_PROPERTY(double threshold READ threshold WRITE setThreshold NOTIFY thresholdChanged)

		static constexpr double INITIAL_THRESHOLD = 0.0;

		Lab4Processor(QObject * parent = nullptr);

		double threshold() const;

		void setThreshold(double threshold);

		bool process(Space & active, Space & bg) override;

	private:
		void processCell(Space & active, Space & bg, int row, int col);

		QColor rule1(Space & active, int row, int col);

		QColor rule2(Space & active, int row, int col);

		QColor rule3(Space & active, int row, int col);

		QColor rule4(Space & active, int row, int col);

//		QColor findDominantNeighbour(Space & active, int row, int col);

	signals:
		void thresholdChanged();

	private:
		double m_threshold;
//		QHash<QRgba64, int> m_colorCounter;
};

#endif
