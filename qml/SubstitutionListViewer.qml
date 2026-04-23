import QtQuick
import QtQuick.Layouts
import com.yoyoviz.cpp 1.0
Item {
    id: root
    required property SubstListModel listModel
    ListView {
        anchors.fill: parent
        id: substitutions
        model: root.listModel
        delegate: Item {
            width: parent.width
            height: row.height
            // height: 300
            id: listItem
            required property var model
            RowLayout { 
                id: row
                width: parent.width
                Rectangle {
                    Layout.alignment: Qt.AlignCenter
                    // Layout.preferredHeight: 200
                    Layout.preferredWidth: parent.width / 2
                    color: "white"
                    Text { 
                        anchors.fill: parent
                        text: listItem.model.substitution.getType()
                        font.pointSize: 12
                    }
                }
                ListView {
                    Layout.preferredWidth: contentWidth
                    Layout.preferredHeight: contentHeight
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    model: listItem.model.substitution.getSubs()
                    delegate: Text{ 
                        required property var model
                        text: model.display
                        font.pointSize: 12
                        color: '#fff'
                    }
                }
            }
        }
    }
}
