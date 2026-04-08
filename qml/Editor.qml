import QtQuick
import QtQuick.Controls
import com.yoyoviz.cpp 1.0
ScrollView {
    id: root
    property alias textDocument:  edit.textDocument
    Highlighter {
        id: highlighter
    }
    background: Rectangle {
        color: "#1e1e2e"
    }
    TextEdit {
        id: edit
        width: root.width
        height: root.height
        font.family: 'JetBrainsMono NF'
        text: 'main: fn = return 4;'
        color: '#cdd6f4'

        Component.onCompleted: {
            highlighter.setDocument(textDocument)
        }
    }
}
