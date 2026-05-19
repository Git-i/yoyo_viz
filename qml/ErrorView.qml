import QtQuick
import com.yoyoviz.cpp
import QtQuick.Layouts
import QtQuick.Controls
Item {
    required property CompilerState compiler
    visible: false
    id: root
    property int currPos: -1
    property int maxPos: 0
    ErrorViewer {
        state: root.compiler
        id: model
    }
    Connections {
        target: root.compiler

        function onStatusChanged() {
            if (root.compiler.status == CompilerState.Error) {
                root.visible = true;
                root.currPos = 0;
                root.maxPos = model.numErrors();
            } else {
                root.currPos = -1;
                root.visible = false
            }
        }
    }
    // compiler.onStatusChanged: {
    //     if (compiler.status == CompilerState.Error) {
    //         root.visible = true;
    //         root.currPos = 0;
    //         root.maxPos = model.numErrors();
    //     } else {
    //         root.currPos = -1;
    //         root.visible = false
    //     }
    // }

    ColumnLayout {
        RowLayout {
            Button {
                text: "Next"
            }
            Button {
                text: "Previous"
            }
            Text {
                text: "Error (" + root.currPos + " / " + root.maxPos + ")"
            }
        }
        Text {
            font.family: 'JetBrainsMono NF'
            font.pointSize: 12
            textFormat: Text.RichText
            text: model.errorAt(root.currPos)
        }
    }
}
