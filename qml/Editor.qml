pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.yoyoviz.cpp 1.0
RowLayout {
    property alias textDocument:  edit.textDocument
    Column { 
        Layout.fillHeight: true
        Layout.fillWidth: false
        width: 30
        Repeater {
            id: lineNumbers
            model: edit.text.split(/\n/g)
            width: 12
            delegate: Label {
                required property int index
                color: '#cdd6f4'
                font: edit.font
                width: parent.width
                height: edit.contentHeight / lineNumbers.count
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
                text: index + 1
                background: Rectangle {
                    border.color: "black"
                    color: "#181825"
                }
            }
        }
    }
    ScrollView {
        Layout.fillHeight: true
        id: editor
        Highlighter {
            id: highlighter
        }
        background: Rectangle {
            color: "#1e1e2e"
        }
        FontMetrics {
            id: fontMetrics
            font: edit.font
        }
        TextEdit {
            id: edit
            width: editor.width
            height: editor.height
            font.family: 'JetBrainsMono NF'
            font.pointSize: 12
            text: 'main: fn -> i32 = return 100;'
            color: '#cdd6f4'
            selectionColor: '#22222244'
            tabStopDistance: fontMetrics.averageCharacterWidth * 4
            Component.onCompleted: {
                highlighter.setDocument(textDocument)
            }
        }
    }
}
