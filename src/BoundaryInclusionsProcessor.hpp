#ifndef BOUNDARYINCLUSIONPROCESSOR_HPP
#define BOUNDARYINCLUSIONPROCESSOR_HPP

#include "AbstractInclusionsProcessor.hpp"

class BoundaryInclusionsProcessor:
	public AbstractInclusionsProcessor
{
	Q_OBJECT

	typedef AbstractInclusionsProcessor Parent;

	public:
		BoundaryInclusionsProcessor(QObject * parent = nullptr);

		bool process(Space & active, Space & bg) override;

	private:
		std::pair<int, int> findBoundaryPos(int row, int col, const Space & space) const;
};

#endif // BOUNDARYINCLUSIONPROCESSOR_HPP
