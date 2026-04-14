import QtQuick
import QtQuick.Controls
import com.yoyoviz.cpp 1.0
Item {
    id: root
    required property ConstraintListModel listModel
    ListView {
        anchors.fill: parent
        id: constraints
        model: root.listModel
        delegate: Column {
            id: col
            required property var model
            Rectangle {
                height: childrenRect.height
                width: 140
                color: '#000'
                Text {
                    font.pointSize: 12
                    color: '#fff'
                    text: col.model.constraint.description()
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        details.visible = !details.visible
                    }
                }
            }
            TableView {
                width: contentWidth
                height: contentHeight
                id: details
                x: 20
                visible: false
                model: col.model.constraint.details()
                rowHeightProvider: function(row) {
                    return -1;
                }
                delegate: TableViewDelegate {
                    font.pointSize: 12
                    padding: 7
                }
                // delegate: Rectangle {
                //     implicitHeight: 50
                //     required property var model
                //     Text {
                //         text: model.display
                //         font.pointSize: 16  // Set your text size here
                //         anchors.centerIn: parent
                //         verticalAlignment: Text.AlignVCenter
                //     }
                // }
            }
        }
    }
}
