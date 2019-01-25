import qbs 1.0

Application {
	name: "CAMP"

	files: [
        "CAMP.qrc",
        "qml/CAMPWindow.qml",
        "qml/DoubleSpinBox.qml",
        "qml/ModelView.qml",
        "src/AbsorbingCell.cpp",
        "src/AbsorbingCell.hpp",
        "src/AbstractInclusionsProcessor.cpp",
        "src/AbstractInclusionsProcessor.hpp",
        "src/AbstractSpaceProcessor.cpp",
        "src/AbstractSpaceProcessor.hpp",
        "src/BoundaryColoringProcessor.cpp",
        "src/BoundaryColoringProcessor.hpp",
        "src/BoundaryInclusionsProcessor.cpp",
        "src/BoundaryInclusionsProcessor.hpp",
        "src/Cell.cpp",
        "src/Cell.hpp",
        "src/DoubleBuffer.hpp",
        "src/DoubleBufferController.hpp",
        "src/InclusionsProcessor.cpp",
        "src/InclusionsProcessor.hpp",
        "src/IsAnyOfTypes.hpp",
        "src/IsIntType.hpp",
        "src/Lab4Processor.cpp",
        "src/Lab4Processor.hpp",
        "src/Model.cpp",
        "src/Model.hpp",
        "src/MooreProcessor.cpp",
        "src/MooreProcessor.hpp",
        "src/NonCopyable.hpp",
        "src/NonMovable.hpp",
        "src/Palette.cpp",
        "src/Palette.hpp",
        "src/Singleton.hpp",
        "src/Space.cpp",
        "src/Space.hpp",
        "src/SpaceProperties.cpp",
        "src/SpaceProperties.hpp",
        "src/main.cpp",
        "src/random.hpp",
    ]

	Depends { name: "cpp" }

	Depends { name: "Qt.qml" }
	Depends { name: "Qt.core" }
	Depends { name: "Qt.quick" }
	Depends { name: "Qt.gui" }
}
