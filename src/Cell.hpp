#ifndef CELL_H
#define CELL_H

#include <QColor>

class Cell
{
    public:
        Cell();

		virtual ~Cell() = default;

		bool hasGrain() const;

		bool hasInclusion() const;

		bool isActive() const;

		bool isEmpty() const;

		bool isFilled() const;

		bool isLocked() const;

		QColor color() const;

		QColor realColor() const;

		virtual void setColor(const QColor & color);

		virtual void setRealColor(const QColor & realColor);

		virtual void switchColor(const QColor & color);

		virtual void setLock(bool lock);

		virtual void switchLock();

		void reset();

		void resetSoft();

    private:
		QColor m_color;
		QColor m_lockColor;
};

#endif // CELL_H
