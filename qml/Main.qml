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
        OutputViewer {
            width: 700
            height: 700
            id: output
            document: editor.textDocument
        }
        Button {
            text: "Compile"
            onClicked: {
                output.compile()
            }
        }
        Editor {
            id: editor
            height: 100
            width: 400
        }
    }
}
