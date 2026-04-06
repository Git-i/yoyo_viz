import QtQuick
import QtQuick.Controls
import com.yoyoviz.cpp 1.0
ScrollView {
    id: root
    Highlighter {
        id: highlighter
    }
    TextEdit {
        width: root.width
        height: root.height
        font.family: 'JetBrainsMono NF'
        text: 'main: fn = return 4;'

        Component.onCompleted: {
            highlighter.setDocument(textDocument)
        }
    }
}
