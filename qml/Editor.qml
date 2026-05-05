pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.yoyoviz.cpp 1.0
Item {
    property alias textDocument: edit.textDocument
    RowLayout {
        anchors.fill: parent

        // Line numbers panel
        Item {
            Layout.fillHeight: true
            Layout.preferredWidth: 40

            clip: true

            Rectangle {
                anchors.fill: parent
                color: "#181825"
            }

            // This column mirrors the TextEdit's contentY so it scrolls in sync
            Column {
                id: lineNumberColumn
                width: parent.width
                y: -flickable.contentY  // Sync scroll position

                Repeater {
                    id: lineNumbers
                    model: edit.text.split(/\n/g).length

                    delegate: Label {
                        required property int index
                        color: '#4c4f69'
                        font: edit.font
                        width: lineNumberColumn.width
                        height: edit.contentHeight / lineNumbers.count
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight
                        rightPadding: 6
                        text: index + 1
                    }
                }
            }
        }

        // Editor
        ScrollView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            id: editor

            // Expose the internal Flickable so we can read contentY
            property alias flickable: flickable

            Highlighter {
                id: highlighter
            }

            background: Rectangle {
                color: "#ccd0da"
            }

            FontMetrics {
                id: fontMetrics
                font: edit.font
            }

            Flickable {
                id: flickable
                anchors.fill: parent
                contentWidth: edit.contentWidth
                contentHeight: edit.contentHeight
                clip: true

                TextEdit {
                    id: edit
                    width: flickable.width
                    font.family: 'JetBrainsMono NF'
                    font.pointSize: 12
                    text: `main: fn -> i32 = {
    val: i32 = 100;
    val2: &i32 = &val;
    if (*val2 == 100) { return 200; }
    return 400;
}`
                    color: '#4c4f69'
                    tabStopDistance: fontMetrics.averageCharacterWidth * 4
                    Component.onCompleted: {
                        highlighter.setDocument(textDocument)
                    }
                }
            }
        }
    }
}
// Item {
//     RowLayout {
//         anchors.fill: parent
//         property alias textDocument:  edit.textDocument
//         Column { 
//             Layout.fillHeight: true
//             Layout.fillWidth: false
//             width: 30
//             Repeater {
//                 id: lineNumbers
//                 model: edit.text.split(/\n/g)
//                 width: 12
//                 delegate: Label {
//                     required property int index
//                     color: '#4c4f69'
//                     font: edit.font
//                     width: parent.width
//                     height: edit.contentHeight / lineNumbers.count
//                     verticalAlignment: Text.AlignVCenter
//                     horizontalAlignment: Text.AlignRight
//                     text: index + 1
//                     background: Rectangle {
//                         color: "#181825"
//                     }
//                 }
//             }
//         }
//         ScrollView {
//             Layout.fillHeight: true
//             Layout.fillWidth: true
//             id: editor
//             Highlighter {
//                 id: highlighter
//             }
//             background: Rectangle {
//                 color: "#ccd0da"
//             }
//             FontMetrics {
//                 id: fontMetrics
//                 font: edit.font
//             }
//             TextEdit {
//                 id: edit
//                 width: editor.width
//                 height: editor.height
//                 font.family: 'JetBrainsMono NF'
//                 font.pointSize: 12
//                 text: `main: fn -> i32 = {
//     val: i32 = 100;
//     val2: &i32 = &val;
//     if (*val2 == 100) { return 200; }
//     return 400;
// }`
//                 color: '#4c4f69'
//                 tabStopDistance: fontMetrics.averageCharacterWidth * 4
//                 Component.onCompleted: {
//                     highlighter.setDocument(textDocument)
//                 }
//             }
//         }
//     }
// }
