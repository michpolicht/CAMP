#ifndef ABSTRACTINCLUSIONPROCESSOR_HPP
#define ABSTRACTINCLUSIONPROCESSOR_HPP

#include "AbstractSpaceProcessor.hpp"

class AbstractInclusionsProcessor:
	public AbstractSpaceProcessor
{
	Q_OBJECT

	typedef AbstractSpaceProcessor Parent;

	public:
		Q_PROPERTY(int inclusions READ inclusions WRITE setInclusions NOTIFY inclusionsChanged)
		Q_PROPERTY(int minInclusionRadius READ minInclusionRadius WRITE setMinInclusionRadius NOTIFY minInclusionRadiusChanged)
		Q_PROPERTY(int maxInclusionRadius READ maxInclusionRadius WRITE setMaxInclusionRadius NOTIFY maxInclusionRadiusChanged)
		Q_PROPERTY(int failedInclusionsLimit READ failedInclusionsLimit WRITE setFailedInclusionsLimit NOTIFY failedInclusionsLimitChanged)

		static constexpr int INITIAL_INCLUSIONS = 0;
		static constexpr int INITIAL_MIN_INCLUSION_RADIUS = 1;
		static constexpr int INITIAL_MAX_INCLUSION_RADIUS = 3;
		static constexpr int INITIAL_FAILED_INCLUSIONS_LIMIT = 1000;

		AbstractInclusionsProcessor(QObject * parent = nullptr);

		int inclusions() const;

		void setInclusions(int inclusions);

		int minInclusionRadius() const;

		void setMinInclusionRadius(int radius);

		int maxInclusionRadius() const;

		void setMaxInclusionRadius(int radius);

		int failedInclusionsLimit() const;

		void setFailedInclusionsLimit(int limit);

	signals:
		void inclusionsChanged();

		void minInclusionRadiusChanged();

		void maxInclusionRadiusChanged();

		void failedInclusionsLimitChanged();

	protected:
		void createInclusion(int row, int col, int radius, Space & space);

		bool hasInclusion(int row, int col, int radius, const Space & space) const;

	private:
		int m_inclusions;
		int m_minInclusionRadius;
		int m_maxInclusionRadius;
		int m_failedInclusionsLimit;
};

#endif // ABSTRACTINCLUSIONPROCESSOR_HPP
