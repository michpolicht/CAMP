#ifndef BOUNDARYCOLORINGPROCESSOR_HPP
#define BOUNDARYCOLORINGPROCESSOR_HPP

#include "AbstractInclusionsProcessor.hpp"

class BoundaryColoringProcessor:
		public AbstractInclusionsProcessor
{
	Q_OBJECT

	typedef AbstractInclusionsProcessor Parent;

	public:
		Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)

		BoundaryColoringProcessor(QObject * parent = nullptr);

		int width() const;

		void setWidth(int width);

		bool process(Space & active, Space & bg) override;

	signals:
		void widthChanged();

	private:
		int m_width;
};

#endif // BOUNDARYCOLORINGPROCESSOR_HPP
