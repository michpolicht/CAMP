#include "Model.hpp"
#include "InclusionsProcessor.hpp"
#include "BoundaryInclusionsProcessor.hpp"
#include "random.hpp"

#include <QColor>
#include <QStringList>
#include <QUrl>
#include <QDir>
#include <QtDebug>
#include <QImage>
#include <QFileInfo>
#include <QImageReader>
#include <QImageWriter>

#include <algorithm>

Model::Model():
	m_space(m_doubleBuffer, Space(& m_spaceProps)),
	m_spaceProcessor(nullptr),
	m_inclusionsProcessor(nullptr),
	m_boundaryColoringProcessor(nullptr)
{
}

int Model::roleId(const QByteArray & name) const
{
	return roleNames().key(name);
}

void Model::step()
{
	qDebug() << "Processing single step of simulation...";

	if (m_spaceProcessor != nullptr)
		m_spaceProcessor->process(m_space.active(), m_space.background());
	else
		qWarning() << "Space processor has not been set.";

	m_doubleBuffer.swapBuffers();
	emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1), { Qt::DisplayRole });

	qDebug() << "Model finished processing single step of simulation.";
}

void Model::fill()
{
	qDebug() << "Processing simulation...";

	if (m_spaceProcessor != nullptr) {
		if (m_space.active().hasGrains()) {
			while (!m_space.active().isFilled()) {
				m_spaceProcessor->process(m_space.active(), m_space.background());
				m_doubleBuffer.swapBuffers();
			}
		} else
			qDebug() << "Space has no nuclei to start with.";
	} else
		qWarning() << "Space processor has not been set.";

	emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1), { Qt::DisplayRole });
	qDebug() << "Model finished processing simulation.";
}

void Model::applyBoundaryInclusions()
{
	if (m_boundaryInclusionsProcessor != nullptr)
		m_boundaryInclusionsProcessor->process(m_space.active(), m_space.background());
	else
		qWarning() << "Boundary inclusions processor has not been set.";
	emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1), { Qt::DisplayRole });

	qDebug() << "Model finished processing boundary inclusions.";
}

void Model::applyInclusions()
{
	if (m_inclusionsProcessor != nullptr)
		m_inclusionsProcessor->process(m_space.active(), m_space.background());
	else
		qWarning() << "Inclusions processor has not been set.";
	emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1), { Qt::DisplayRole });

	qDebug() << "Model finished processing inclusions.";
}

bool Model::toggleDataLock(const QModelIndex & index)
{
	bool result = m_space.active().toggleGrainLock(index.row(), index.column());
	m_space.background() = m_space.active();
	emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1), { LockedRole });
	return result;
}

void Model::applyBoundaryColoring()
{
	if (m_boundaryColoringProcessor != nullptr)
		m_boundaryColoringProcessor->process(m_space.active(), m_space.background());
	else
		qWarning() << "Coundary coloring processor has not been set.";
	emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1), { Qt::DisplayRole });

	qDebug() << "Boundary coloring finished.";
}

int Model::rowCount(const QModelIndex & parent) const
{
	Q_UNUSED(parent)

	return m_spaceProps.rows();
}

int Model::columnCount(const QModelIndex & parent) const
{
	Q_UNUSED(parent)

	return m_spaceProps.columns();
}

SpaceProperties::BoundaryType Model::boundaryType() const
{
	return m_spaceProps.boundaryType();
}

void Model::setBoundaryType(SpaceProperties::BoundaryType type)
{
	if (m_spaceProps.boundaryType() != type) {
		m_spaceProps.setBoundaryType(type);
		emit boundaryTypeChanged();
	}
}

AbstractSpaceProcessor * Model::spaceProcessor() const
{
	return m_spaceProcessor;
}

void Model::setSpaceProcessor(AbstractSpaceProcessor * spaceProcessor)
{
	if (m_spaceProcessor != spaceProcessor) {
		m_spaceProcessor = spaceProcessor;
		emit spaceProcessorChanged();
	}
}

AbstractInclusionsProcessor *Model::boundaryInclusionsProcessor() const
{
	return m_boundaryInclusionsProcessor;
}

void Model::setBoundaryInclusionsProcessor(AbstractInclusionsProcessor * inclusionsProcessor)
{
	if (m_boundaryInclusionsProcessor != inclusionsProcessor) {
		m_boundaryInclusionsProcessor = inclusionsProcessor;
		emit boundaryInclusionsProcessorChanged();
	}
}

AbstractInclusionsProcessor *Model::inclusionsProcessor() const
{
	return m_inclusionsProcessor;
}

void Model::setInclusionsProcessor(AbstractInclusionsProcessor * inclusionsProcessor)
{
	if (m_inclusionsProcessor != inclusionsProcessor) {
		m_inclusionsProcessor = inclusionsProcessor;
		emit inclusionsProcessorChanged();
	}
}

BoundaryColoringProcessor *Model::boundaryColoringProcessor() const
{
	return m_boundaryColoringProcessor;
}

void Model::setBoundaryColoringProcessor(BoundaryColoringProcessor * coloringProcessor)
{
	if (m_boundaryColoringProcessor != coloringProcessor) {
		m_boundaryColoringProcessor = coloringProcessor;
		emit boundaryColoringProcessorChanged();
	}
}

QVariant Model::data(const QModelIndex & index, int role) const
{
	if (index.isValid())
		switch (role) {
			case Qt::DisplayRole:
				return m_space.active(index.row(), index.column()).color();
			case LockedRole:
				return m_space.active(index.row(), index.column()).isLocked();
			case RealColorRole:
				return m_space.active(index.row(), index.column()).realColor();
			default:
				break;
		}
	return QVariant();
}

bool Model::setData(const QModelIndex & index, const QVariant & value, int role)
{
	m_space.active(index.row(), index.column()).switchColor(value.value<QColor>());
	m_space.background(index.row(), index.column()).setColor(m_space.active(index.row(), index.column()).color());
	if (m_space.active(index.row(), index.column()).hasGrain())
		emit nucleiFormed();
	emit dataChanged(index, index, { role });
	return true;
}

QHash<int, QByteArray> Model::roleNames() const
{
	return {
		{Qt::DisplayRole, "cellColor"},
		{LockedRole, "locked"},
		{RealColorRole, "realColor"}
	};
}

void Model::toggleRandomPoint(const QColor & color)
{
	int row = random(0, rowCount() - 1);
	int column = random(0, columnCount() - 1);
	if (!m_space.active(row, column).hasGrain()) {
		m_space.active(row, column).setColor(color);
		m_space.background(row, column).setColor(color);
		if (m_space.active(row, column).hasGrain())
			emit nucleiFormed();
		emit dataChanged(createIndex(row, column), createIndex(row, column), { Qt::DisplayRole });
	} else
		qDebug() << "Picked random point, which already has a grain. Good luck next time!";
}

void Model::setRows(int rows)
{
	if (m_spaceProps.rows() > rows) {
		beginRemoveRows(QModelIndex(), rows , m_spaceProps.rows() - 1);
		m_spaceProps.setRows(rows);
		endRemoveRows();
		emit rowsChanged();
	} else if (m_spaceProps.rows() < rows) {
		beginInsertRows(QModelIndex(), m_spaceProps.rows(), rows  - 1);
		m_spaceProps.setRows(rows);
		endInsertRows();
		emit rowsChanged();
	}
}

void Model::setColumns(int columns)
{
	if (m_spaceProps.columns() > columns) {
		beginRemoveColumns(QModelIndex(), columns, m_spaceProps.columns() - 1);
		m_spaceProps.setColumns(columns);
		endRemoveColumns();
		emit columnsChanged();
	} else if (m_spaceProps.columns() < columns) {
		beginInsertColumns(QModelIndex(), m_spaceProps.columns(), columns - 1);
		m_spaceProps.setColumns(columns);
		endInsertColumns();
		emit columnsChanged();
	}
}

void Model::reset()
{
	m_space.active().reset();
	m_space.background().reset();
	emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1), { Qt::DisplayRole, LockedRole, RealColorRole });
}

void Model::resetSoft()
{
	m_space.active().resetSoft();
	m_space.background().resetSoft();
	emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1), { Qt::DisplayRole });
}

void Model::unifyLocked()
{
	m_space.active().unifyLocked();
	m_space.background() = m_space.active();
	emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1), { Qt::DisplayRole });
}

bool Model::exportImage(const QString & url) const
{
	qDebug() << "Exporting image to " << url;

	QImage image(columnCount(), rowCount(), QImage::Format_ARGB32);
	for (int row = 0; row < columnCount(); row++)
		for (int column = 0; column < columnCount(); column++)
			image.setPixelColor(column, row, m_space.active(row, column).color());

	QString filePath = QUrl(url).toLocalFile();
	QDir path(filePath);

	qDebug() << "Exporting image to " << path.absolutePath() << "...";
	if (image.save(path.absolutePath())) {
		qDebug() << "Succesfuly exported image to file " << path.absolutePath() << ".";
		return true;
	} else {
		qDebug() << "Failed to export image to file " << path.absolutePath() << ".";
		return false;
	}
}

bool Model::importImage(const QString & url)
{
	qDebug() << "Importing image from " << url;

	QImage image;
	QString filePath = QUrl(url).toLocalFile();
	QDir path(filePath);

	qDebug() << "Importing image from " << path.absolutePath() << "...";
	if (!image.load(path.absolutePath())) {
		qDebug() << "Failed to import image from file " << path.absolutePath() << ".";
		return false;
	}
	qDebug() << "Succesfuly imported image from file " << path.absolutePath() << ".";

	qDebug() << "Setting model size to fit the data.";
	setRows(image.height());
	setColumns(image.width());

	qDebug() << "Restoring data from " << path.absolutePath() << "...";
	for (int row = 0; row < image.height(); row++)
		for (int col = 0; col < image.width(); col++) {
			m_space.active(row, col).setColor(image.pixelColor(col, row));
			m_space.background(row, col).setColor(image.pixelColor(col, row));
		}

	qDebug() << "Succesfuly restored data from " << path.absolutePath() << ".";
	emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1), { Qt::DisplayRole });

	return true;
}

bool Model::exportFile(const QString & url) const
{
	QFileInfo fileInfo(QUrl(url).toLocalFile());
	if (QImageWriter::supportedImageFormats().contains(fileInfo.suffix().toLower().toLocal8Bit()))
		return exportImage(url);
	else
		return exportCSV(url);
}

bool Model::importFile(const QString & url)
{
	QFileInfo fileInfo(QUrl(url).toLocalFile());
	if (QImageReader::supportedImageFormats().contains(fileInfo.suffix().toLower().toLocal8Bit()))
		return importImage(url);
	else
		return importCSV(url);
}

bool Model::exportCSV(const QString & url) const
{
	qDebug() << "Exporting CSV to " << url;
	qDebug() << QImageReader::supportedImageFormats().contains("bmp");

	QString filePath = QUrl(url).toLocalFile();
	QDir path(filePath);
	QFile dataFile(path.absolutePath());

	if (!dataFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qWarning(dataFile.errorString().toLocal8Bit().constData());
		return false;
	}

	qDebug() << "Saving data to " << path.absolutePath() << "...";
	QTextStream dataStream(& dataFile);
	for (int row = 0; row < rowCount(); row++)
		for (int col = 0; col < columnCount(); col++)
			dataStream << row << ", " << col << ", " << static_cast<quint64>(m_space.active(row, col).color().rgba64()) << "\n";

	dataFile.close();

	qDebug() << "Succesfuly saved data to " << path.absolutePath() << ".";
	return true;
}

bool Model::importCSV(const QString & url)
{
	typedef QList<int> RowDataContainer;
	typedef QList<int> ColumnDataContainer;
	typedef QList<quint64> RgbaDataContainer;

	static const char * COULD_NOT_CONVERT_ARG_TO_INT = "Could not convert '%1' to int";
	static const char * COULD_NOT_CONVERT_ARG_TO_ULONGLONG = "Could not convert '%1' to qulonglong";

	qDebug() << "Importing CSV from " << url;
	QString filePath = QUrl(url).toLocalFile();
	QDir path(filePath);
	QFile dataFile(path.absolutePath());

	qDebug() << "Restoring data from " << path.absolutePath() << "...";
	if (!dataFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qWarning(dataFile.errorString().toLocal8Bit().constData());
		return false;
	}

	// Read data to temporary structures for easier and more effective manipulation.
	RowDataContainer rowData;
	ColumnDataContainer columnData;
	RgbaDataContainer rgbaData;
	try {
		bool ok;
		QString line;
		QTextStream dataStream(& dataFile);

		while (dataStream.readLineInto(& line)) {
			QStringList splitLine = line.split(',');

			int row = splitLine.at(0).toInt(& ok);
			if (!ok)
				throw  std::runtime_error(QString(COULD_NOT_CONVERT_ARG_TO_INT).arg(splitLine.at(0)).toStdString());

			int col = splitLine.at(1).toInt(& ok);
			if (!ok)
				throw  std::runtime_error(QString(COULD_NOT_CONVERT_ARG_TO_INT).arg(splitLine.at(0)).toStdString());

			quint64 rgba = splitLine.at(2).toULongLong(& ok);
			if (!ok)
				throw  std::runtime_error(QString(COULD_NOT_CONVERT_ARG_TO_ULONGLONG).arg(splitLine.at(0)).toStdString());

			qDebug() << "Appending " << row << ", " << col << ", " << rgba;
			rowData.append(row);
			columnData.append(col);
			rgbaData.append(rgba);
		}
	} catch (const std::runtime_error & e) {
		qWarning(e.what());
		dataFile.close();
		return false;
	}
	dataFile.close();

	// Find number of rows and columns.
	int rowMax = *std::max_element(rowData.begin(), rowData.end()) + 1;
	int columnMax = *std::max_element(columnData.begin(), columnData.end()) + 1;
	qDebug() << "Space size: " << rowMax << ", " << columnMax;

	qDebug() << "Setting model size to fit the data.";
	setRows(rowMax);
	setColumns(columnMax);

	// Just in case data is incomplete, reset the model.
	reset();

	RowDataContainer::const_iterator row = rowData.begin();
	ColumnDataContainer::const_iterator col = columnData.begin();
	RgbaDataContainer::const_iterator rgba = rgbaData.begin();
	// All containers must have same size, if they have passed data read.
	while (row != rowData.end()) {
		m_space.active(*row, *col).setColor(qRgba64(*rgba));
		m_space.background(*row, *col).setColor(qRgba64(*rgba));
		++row;
		++col;
		++rgba;
	}

	qDebug() << "Succesfuly restored data from " << path.absolutePath() << ".";
	emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1), { Qt::DisplayRole });

	return true;
}
