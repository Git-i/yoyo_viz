import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import com.yoyoviz.cpp 1.0
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
    ColumnLayout {
        anchors.fill: parent
        RowLayout {

            Button {
                text: "Compile"
                onClicked: {
                    output.compile()
                }
            }
            Text {
                text: {
                    switch(output.compiler.status) {
                        case CompilerState.Empty: return "Empty";
                        case CompilerState.Ready: return "Ready";
                        case CompilerState.Error: return "Error";
                        case CompilerState.Compiling: return "Compiling...";
                    }
                }
                font.pointSize: 12
            }
        }
        SplitView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            id: main_col
            Editor {
                SplitView.preferredWidth: 600
                SplitView.fillHeight: true
                id: editor
            }
            ColumnLayout {
                OutputViewer {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    id: output
                    document: editor.textDocument
                }
                ErrorView {
                    compiler: output.compiler
                    Layout.preferredHeight: 400
                }
            }
        }
    }
}
