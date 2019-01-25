import QtQuick 2.12
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

import CAMP 0.1 as CAMP

ApplicationWindow {
	id: appWindow

	visible: true
	width: 1000
	height: 800
	title: qsTr("CAMP")

	header: ToolBar {
		RowLayout {
//			spacing: 20

			ToolButton {
				contentItem: Image {
					fillMode: Image.Pad
					horizontalAlignment: Image.AlignHCenter
					verticalAlignment: Image.AlignVCenter
					source: "qrc:/images/menu.png"
				}

				onClicked: optionsMenu.open()

				Menu {
					id: optionsMenu
					//					x: parent.width - width
					y: parent.height
					transformOrigin: Menu.TopRight

					MenuItem {
						text: "Export..."
						onTriggered: fileExportDialog.open()
					}

					MenuItem {
						text: "Import..."
						onTriggered: fileImportDialog.open()
					}
				}
			}

			Button {
				text: qsTr("Reset")

				onClicked: model.reset()
			}

			Button {
				text: qsTr("Clear Unlocked")

				onClicked: model.resetSoft()
			}

			Button {
				text: qsTr("Unify locked")

				onClicked: model.unifyLocked()
			}

			Button {
				text: qsTr("Step")

				onClicked: model.step()
			}

			Button {
				text: qsTr("Fill")

				onClicked: model.fill()
			}

			Button {
				text: qsTr("Apply initial inclusions")

				onClicked: model.applyInclusions()
			}

			Button {
				text: qsTr("Apply inclusions")

				onClicked: model.applyBoundaryInclusions()
			}

			Button {
				text: qsTr("Appply boundary coloring")

				onClicked: model.applyBoundaryColoring()
			}
		}
	}

	property var model: CAMP.Model {
		rows: 40
		columns: 50
		boundaryType: boundaryTypeComboBox.currentIndex
		spaceProcessor: CAMP.Lab4Processor { threshold: thresholdSpinBox.realValue }
		inclusionsProcessor: CAMP.InclusionsProcessor {}
		boundaryInclusionsProcessor: CAMP.BoundaryInclusionsProcessor {}
		boundaryColoringProcessor: CAMP.BoundaryColoringProcessor { width: 3 }

		onNucleiFormed: colorPicker.incrementCurrentIndex()
	}

	ColumnLayout {
		anchors.margins: 20
		anchors.fill: parent
		spacing: 5

		ListView {
			id: colorPicker

			height: boundaryTypeComboBox.height
			focus: true
			orientation: Qt.Horizontal
			model: 50
			highlightMoveDuration: 100

			delegate: Rectangle {
				width: height * 2
				height: colorPicker.height
				radius: height * 0.5
				color: Qt.hsla((colorPicker.colorDelta * index) % 1.0, 1.0, 0.5, 1.0)
				border.width: 2
				border.color: "white"

				MouseArea {
					anchors.fill: parent

					onClicked: colorPicker.currentIndex = index
				}
			}

			highlight: Rectangle {
				z: 2
				border.width: 8
				radius: height * 0.5
				color: "transparent"
				border.color: "black"
			}

			Layout.fillWidth: true
			Layout.alignment: Qt.AlignTop

			property color color: currentItem.color
			readonly property real colorDelta: 0.29
		}


		RowLayout {
			Layout.fillWidth: true
			Layout.fillHeight: true

			Slider {
				id: grainSizeSlider

				Layout.alignment: Qt.AlignLeft
				Layout.fillHeight: true

				orientation: Qt.Vertical
				from: 1
				to: 50
				stepSize: 1
				value: 10
			}

			Pane {
				Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
				Layout.fillWidth: true
				Layout.fillHeight: true

				ModelView {
					id: modelView

					model: appWindow.model

					activeColor: colorPicker.color
					grainSize: grainSizeSlider.value
				}
			}
		}

		RowLayout {
			spacing: 5

			Layout.alignment: Qt.AlignBottom

			RowLayout {
				Label {
					text: qsTr("Columns:")
				}

				SpinBox {
					id: spaceWidthSpinBox

					value: model.columns
					editable: true
					from: 1
					to: 2000

					onValueChanged: model.columns = value
				}
			}

			RowLayout {
				Label {
					text: qsTr("Rows:")
				}

				SpinBox {
					id: spaceHeightSpinBox

					value: model.rows
					editable: true
					from: 1
					to: 2000

					onValueChanged: model.rows = value
				}
			}

			RowLayout {
				Label {
					text: qsTr("Space type:")
				}

				ComboBox {
					id: boundaryTypeComboBox

					currentIndex: 0
					model: [qsTr("Absorbing"), qsTr("Periodic")]
				}

			}
		}

		RowLayout {
			spacing: 5

			Layout.alignment: Qt.AlignBottom

			RowLayout {
				Label {
					text: qsTr("Inclusions:")
				}

				SpinBox {
					id: inclusionSpinBox

					value: model.inclusionsProcessor.inclusions
					editable: true
					from: 0
					to: 2000

					onValueChanged: model.inclusionsProcessor.inclusions = value
				}
			}


			RowLayout {
				Label {
					text: qsTr("Inclusion radius:")
				}

				RangeSlider {
					from: 1
					to: 10
					stepSize: 1
					snapMode: RangeSlider.SnapAlways
					first.value: model.inclusionsProcessor.minInclusionRadius
					second.value: model.inclusionsProcessor.maxInclusionRadius

					first.onValueChanged: model.inclusionsProcessor.minInclusionRadius = first.value
					second.onValueChanged: model.inclusionsProcessor.maxInclusionRadius = second.value
				}
			}

			RowLayout {
				Label {
					text: qsTr("Threshold:")
				}

				DoubleSpinBox {
					id: thresholdSpinBox

					value: model.threshold
					editable: true
					from: 0
					to: 100
				}
			}


		}


		RowLayout {
			spacing: 5

			Layout.alignment: Qt.AlignBottom

			RowLayout {
				Label {
					text: qsTr("Nuclei amount:")
				}

				SpinBox {
					id: nucleiAmountSpinBox

					value: 1
					editable: true
					from: 1
					to: 2000
				}
			}

			Button {
				text: qsTr("Random")

				onClicked: {
					for (var i = 0; i < nucleiAmountSpinBox.value; i++)
						model.toggleRandomPoint(colorPicker.color)
				}
			}

//			Button {
//				text: "Temporary export CSV"

//				onClicked: model.exportCSV("file:///C:/Users/doc/Documents/CA_MIP.mingw32/data/test.txt")
//			}

//			Button {
//				text: "Temporary import CSV"

//				onClicked: model.importCSV("file:///C:/Users/doc/Documents/CA_MIP.mingw32/data/small")
//			}

//			Button {
//				text: "Temporary export image"

//				onClicked: model.exportImage("file:///C:/Users/doc/Documents/CA_MIP.mingw32/data/test.png")
//			}

//			Button {
//				text: "Temporary import image"

//				onClicked: model.importImage("file:///C:/Users/doc/Documents/CA_MIP.mingw32/data/small.png")
//			}

//			Button {
//				text: "Smaller"

//				onClicked: {
//					model.rows = 20
//					model.columns = 20
//				}
//			}
//			Button {
//				text: "Larger"

//				onClicked: {
//					model.rows = 50
//					model.columns = 50
//				}
//			}
		}
	}

	FileDialog {
		id: fileExportDialog

		title: "Please choose a file"
		folder: shortcuts.home
		selectExisting: false

		onAccepted: model.exportFile(fileUrl)

		onRejected: console.log("Canceled")
	}

	FileDialog {
		id: fileImportDialog

		title: "Please choose a file"
		folder: shortcuts.home
		selectExisting: true

		onAccepted: model.importFile(fileUrl)

		onRejected: console.log("Canceled")
	}
}
