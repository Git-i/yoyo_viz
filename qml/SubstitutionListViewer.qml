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
            required property int index
            Rectangle {
                anchors.fill: parent
                color: parent.index % 2 == 0 ? "#eff1f5" : "#e6e9ef"
            }
            RowLayout { 
                id: row
                width: parent.width
                Text { 
                    Layout.fillHeight: true
                    Layout.preferredWidth: row.width / 2
                    text: listItem.model.substitution.getType()
                    font.pointSize: 12
                    color: "#4c4f69"
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
                        color: '#4c4f69'
                    }
                }
            }
        }
    }
}
