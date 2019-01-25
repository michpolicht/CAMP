#ifndef MODEL_HPP
#define MODEL_HPP

#include "Space.hpp"
#include "SpaceProperties.hpp"
#include "AbstractSpaceProcessor.hpp"
#include "AbstractInclusionsProcessor.hpp"
#include "BoundaryColoringProcessor.hpp"

#include <QAbstractTableModel>

#include <memory>

class Model:
	public QAbstractTableModel
{
	Q_OBJECT

	public:
		enum Roles {
			LockedRole = Qt::UserRole,
			RealColorRole
		};

		Q_PROPERTY(int rows READ rowCount WRITE setRows NOTIFY rowsChanged)
		Q_PROPERTY(int columns READ columnCount WRITE setColumns NOTIFY columnsChanged)
		Q_PROPERTY(SpaceProperties::BoundaryType boundaryType READ boundaryType WRITE setBoundaryType NOTIFY boundaryTypeChanged)
		Q_PROPERTY(AbstractSpaceProcessor * spaceProcessor READ spaceProcessor WRITE setSpaceProcessor NOTIFY spaceProcessorChanged)
		Q_PROPERTY(AbstractInclusionsProcessor * boundaryInclusionsProcessor READ boundaryInclusionsProcessor WRITE setBoundaryInclusionsProcessor NOTIFY boundaryInclusionsProcessorChanged)
		Q_PROPERTY(AbstractInclusionsProcessor * inclusionsProcessor READ inclusionsProcessor WRITE setInclusionsProcessor NOTIFY inclusionsProcessorChanged)
		Q_PROPERTY(BoundaryColoringProcessor * boundaryColoringProcessor READ boundaryColoringProcessor WRITE setBoundaryColoringProcessor NOTIFY boundaryColoringProcessorChanged)
//		Q_PROPERTY(QColor emptyColor READ emptyColor CONSTANT)

		Model();

		Q_INVOKABLE int roleId(const QByteArray & name) const;

		Q_INVOKABLE void step();

		Q_INVOKABLE void fill();

		Q_INVOKABLE void applyBoundaryInclusions();

		Q_INVOKABLE void applyInclusions();

		Q_INVOKABLE bool toggleDataLock(const QModelIndex & index);

		Q_INVOKABLE void applyBoundaryColoring();

		int rowCount(const QModelIndex & parent = QModelIndex()) const override;

		int columnCount(const QModelIndex & parent = QModelIndex()) const override;

		SpaceProperties::BoundaryType boundaryType() const;

		void setBoundaryType(SpaceProperties::BoundaryType boundaryType);

		AbstractSpaceProcessor * spaceProcessor() const;

		void setSpaceProcessor(AbstractSpaceProcessor * spaceProcessor);

		AbstractInclusionsProcessor * boundaryInclusionsProcessor() const;

		void setBoundaryInclusionsProcessor(AbstractInclusionsProcessor * inclusionsProcessor);

		AbstractInclusionsProcessor * inclusionsProcessor() const;

		void setInclusionsProcessor(AbstractInclusionsProcessor * inclusionsProcessor);

		BoundaryColoringProcessor * boundaryColoringProcessor() const;

		void setBoundaryColoringProcessor(BoundaryColoringProcessor * coloringProcessor);

		QVariant data(const QModelIndex & index, int role) const override;

		bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;

		QHash<int, QByteArray> roleNames() const override;

	public slots:
		void toggleRandomPoint(const QColor & color);

		void setRows(int rows);

		void setColumns(int columns);

		void reset();

		void resetSoft();

		void unifyLocked();

		bool exportCSV(const QString & url) const;

		bool importCSV(const QString & url);

		bool exportImage(const QString & url) const;

		bool importImage(const QString & url);

		bool exportFile(const QString & url) const;

		bool importFile(const QString & url);

	signals:
		void rowsChanged();

		void columnsChanged();

		void boundaryTypeChanged();

		void spaceProcessorChanged();

		void boundaryInclusionsProcessorChanged();

		void inclusionsProcessorChanged();

		void boundaryColoringProcessorChanged();

		void nucleiFormed();

	private:
		SpaceProperties m_spaceProps;
		DoubleBufferController m_doubleBuffer;
		DoubleBuffer<Space> m_space;
		AbstractSpaceProcessor * m_spaceProcessor;
		AbstractInclusionsProcessor * m_boundaryInclusionsProcessor;
		AbstractInclusionsProcessor * m_inclusionsProcessor;
		BoundaryColoringProcessor * m_boundaryColoringProcessor;
};

#endif // MODEL_HPP
