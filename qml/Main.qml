import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
Window {
    id: root
    width: 1280
    height: 720
    visible: true
    title: 'Hello World'

    palette: Palette {
        window: "#eff1f5"       // Base
        windowText: "#4c4f69"   // Text
        base: "#e6e9ef"         // Mantle
        alternateBase: "#ccd0da" // Crust
        text: "#4c4f69"         // Text
        button: "#e6e9ef"       // Surface0
        buttonText: "#4c4f69"   // Text
        highlight: "#1e66f5"    // Blue
        highlightedText: "#eff1f5" // Base
        brightText: "#fe640b"   // Peach
        link: "#7287fd"         // Lavender
        disabled.text: "#9ca0b0" // Overlay0
        disabled.buttonText: "#9ca0b0"
    }
    color: '#eff1f5'
    SplitView {
        anchors.fill: parent
        id: main_col
        ColumnLayout {
            SplitView.preferredWidth: 100
            Button {
                text: "Compile"
                onClicked: {
                    output.compile()
                }
            }
            Editor {
                Layout.fillWidth: true
                Layout.fillHeight: true
                id: editor
            }
        }
        OutputViewer {
            SplitView.fillHeight: true
            SplitView.fillWidth: true
            id: output
            document: editor.textDocument
        }
    }
}
