import QtQuick 2.12
import QtQuick.Controls 2.12

import CAMP 0.1 as CAMP

TableView {
	id: view

	anchors.centerIn: parent
	implicitWidth: contentWidth
	implicitHeight: contentHeight
	width: parent.width > implicitWidth ? implicitWidth : parent.width
	height: parent.height > implicitHeight ? implicitHeight : parent.height
	columnSpacing: 1
	rowSpacing: 1
	clip: true
	columnWidthProvider: function() { return grainSize }
	rowHeightProvider: function() { return grainSize }

	property color activeColor: "red"
	property int grainSize: 10

	onGrainSizeChanged: forceLayout()

	delegate: Rectangle {
		// <workaround>
		// TableView seems to update old delegates, whenever dataChanged() is
		// emitted. When model has been resized and subsequently dataChanged()
		// has been called (like during imports) it attempts to receive data
		// for non existing cells.
		// Insead of
		//		color: cellColor
		color: cellColor === undefined ? "black" :
										 locked ? Qt.darker(realColor) : realColor
		// </workaround>

		MouseArea {
			anchors.fill: parent
			onClicked: {
				if (view.model.setData(view.model.index(row, column), activeColor, view.model.roleId("cellColor")))
					console.log("Set data at (" + row + ", " + column + ").")
				else
					console.log("Failed to set data at (" + row + ", " + column + ").")
			}

			onPressAndHold: {
				if (view.model.toggleDataLock(view.model.index(row, column)))
					console.log("Grain at (" + row + ", " + column + ") locked.")
				else
					console.log("Grain at (" + row + ", " + column + ") unlocked.")
			}
		}
	}
}
