import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
Window {
    id: root
    width: 1280
    height: 720
    visible: true
    title: 'Hello World'

    color: '#1e1e2e'
    ColumnLayout {
        anchors.fill: parent
        id: main_col
        Editor {
            id: editor
            height: 200
            width: 700
        }
        OutputViewer {
            width: 700
            height: 300
            id: output
            document: editor.textDocument
        }
        Button {
            text: "Compile"
            onClicked: {
                output.compile()
            }
        }
    }
}
