import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import com.yoyoviz.cpp 1.0
Item {
    id: root
    required property TextDocument document
    property var compile
    Component.onCompleted: {
        compile = function () {
            state.compile()
        }
    }
    CompilerState {
        id: state
        document: root.document
    }
    Column {
        width: parent.width
        height: parent.height
        Text {
            text: {
                switch(state.status) {
                    case CompilerState.Empty: return "Empty";
                    case CompilerState.Ready: return "Ready";
                    case CompilerState.Error: return "Error";
                    case CompilerState.Compiling: return "Compiling...";
                }
            }
            color: "#fff"
            font.pointSize: 12
        }
        TabBar {
            id: tab
            width: parent.width
            TabButton {
                text: "Parser Output"
            }
            TabButton {
                text: "Type Checker Output"
            }
        }
        StackLayout {
            width: parent.width
            height: parent.height
            ParserOutput {
                id: parserViewer
                state: state
            }
            TypeCheckerViewer {
                compiler: state
            }
            currentIndex: tab.currentIndex
        }
    }
}
