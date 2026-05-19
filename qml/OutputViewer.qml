import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import com.yoyoviz.cpp 1.0
Item {
    id: root
    required property TextDocument document
    property var compile
    property alias compiler: state
    Component.onCompleted: {
        compile = function () {
            state.compile()
        }
    }
    CompilerState {
        id: state
        document: root.document
    }
    ColumnLayout {
        anchors.fill: parent
        TabBar {
            id: tab
            Layout.fillWidth: true
            TabButton {
                text: "Parser Output"
            }
            TabButton {
                text: "Type Checker Output"
            }
            TabButton {
                text: "Borrow Checker Ouput"
            }
        }
        StackLayout {
            Layout.fillWidth: true; Layout.fillHeight: true
            ParserOutput {
                id: parserViewer
                state: state
            }
            TypeCheckerViewer {
                Layout.fillHeight: true; Layout.fillWidth: true
                compiler: state
            }
            BorrowCheckerViewer {
                compiler: state
            }
            currentIndex: tab.currentIndex
        }
    }
}
