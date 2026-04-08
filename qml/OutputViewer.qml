import QtQuick
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
    ParserOutput {
        id: parserViewer
        state: state
    }
    Text {
        text: state.status
        color: "#fff"
        font.pointSize: 12
    }
}
