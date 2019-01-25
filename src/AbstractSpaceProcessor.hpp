#ifndef IPROCESSOR_HPP
#define IPROCESSOR_HPP

#include "Space.hpp"

class AbstractSpaceProcessor:
	public QObject
{
	Q_OBJECT

	public:
		AbstractSpaceProcessor(QObject * parent = nullptr);

		virtual bool process(Space & active, Space & bg) = 0;

	protected:
		virtual ~AbstractSpaceProcessor() = default;
};

#endif // IPROCESSOR_HPP
