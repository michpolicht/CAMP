import QtQuick.Controls 2.0
import QtQuick 2.12

SpinBox {
	 id: spinbox

	 from: 0
	 value: 25
	 to: 100
	 stepSize: 10

	 property int decimals: 2
	 property real realValue: value / 100

	 validator: DoubleValidator {
		 bottom: Math.min(spinbox.from, spinbox.to)
		 top:  Math.max(spinbox.from, spinbox.to)
	 }

	 textFromValue: function(value, locale) {
		 return Number(value / 100).toLocaleString(locale, 'f', spinbox.decimals)
	 }

	 valueFromText: function(text, locale) {
		 return Number.fromLocaleString(locale, text) * 100
	 }
 }
