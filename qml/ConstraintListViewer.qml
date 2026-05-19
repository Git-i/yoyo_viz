import QtQuick
import QtQuick.Controls
import com.yoyoviz.cpp 1.0
Item {
    id: root
    required property ConstraintListModel listModel
    // Rectangle {
    //     anchors.fill: parent
    //     color: '#8c8fa1'
    // }
    ListView {
        anchors.fill: parent
        id: constraints
        model: root.listModel
        delegate: Item {
            required property var model
            required property var index
            id: col
            width: ListView.view.width; height: col_child.height

            Column {
                id: col_child
                Rectangle {
                    height: childrenRect.height
                    width: col.width
                    color: col.index % 2 == 0 ? "#eff1f5" : "#e6e9ef"
                    Text {
                        font.pointSize: 12
                        // color: '#4c4f69'
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
}
