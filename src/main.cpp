#include "Model.hpp"
#include "SpaceProperties.hpp"
#include "AbstractSpaceProcessor.hpp"
#include "AbstractInclusionsProcessor.hpp"
#include "InclusionsProcessor.hpp"
#include "BoundaryInclusionsProcessor.hpp"
#include "MooreProcessor.hpp"
#include "Lab4Processor.hpp"
#include "BoundaryColoringProcessor.hpp"
#include "Palette.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>

QObject * paletteProvider(QQmlEngine * engine, QJSEngine * scriptEngine)
{
	Q_UNUSED(scriptEngine)

	Palette * palette = & Palette::Instance();
	engine->setObjectOwnership(palette, QQmlEngine::CppOwnership);
	return palette;
}

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QCoreApplication::setOrganizationName("AGH");
	QGuiApplication app(argc, argv);

	qmlRegisterType<Model>("CAMP", 0, 1, "Model");
	qmlRegisterUncreatableType<SpaceProperties>("CAMP", 0, 1, "SpaceProperties", "Can not create SpaceProperties object from QML.");
	qmlRegisterUncreatableType<AbstractSpaceProcessor>("CAMP", 0, 1, "AbstractSpaceProcessor", "Can not create AbstractSpaceProcessor object from QML.");
	qmlRegisterUncreatableType<AbstractInclusionsProcessor>("CAMP", 0, 1, "AbstractInclusionsProcessor", "Can not create AbstractInclusionsProcessor object from QML.");
	qmlRegisterType<MooreProcessor>("CAMP", 0, 1, "MooreProcessor");
	qmlRegisterType<InclusionsProcessor>("CAMP", 0, 1, "InclusionsProcessor");
	qmlRegisterType<BoundaryInclusionsProcessor>("CAMP", 0, 1, "BoundaryInclusionsProcessor");
	qmlRegisterType<MooreProcessor>("CAMP", 0, 1, "MooreProcessor");
	qmlRegisterType<Lab4Processor>("CAMP", 0, 1, "Lab4Processor");
	qmlRegisterType<BoundaryColoringProcessor>("CAMP", 0, 1, "BoundaryColoringProcessor");
	qmlRegisterSingletonType<Palette>("CAMP", 0, 1, "Palette", paletteProvider);

	QQmlApplicationEngine engine;
	engine.load(QUrl(QLatin1String("qrc:/qml/CAMPWindow.qml")));

	return app.exec();
}
