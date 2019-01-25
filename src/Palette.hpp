#ifndef PALETTE_HPP
#define PALETTE_HPP

#include "Singleton.hpp"

#include <QObject>
#include <QColor>

class Palette:
	public QObject,
	public Singleton<Palette>
{
	Q_OBJECT

	friend class Singleton<Palette>;

	public:
		Q_INVOKABLE QColor empty() const;

		Q_INVOKABLE QColor inclusion() const;

	protected:
		Palette();
};

#endif // PALETTE_HPP
