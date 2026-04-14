import QtQuick
import QtQuick.Layouts
import com.yoyoviz.cpp 1.0
Item {
    id: root
    required property CompilerState compiler
    TypeCheckerModel {
        id: checker
        functionName: "source::main"
        Component.onCompleted: {
            this.setCompiler(root.compiler)
        }
    }
    Loader {
        anchors.fill: parent
        sourceComponent: {
            switch(root.compiler.status) {
                case CompilerState.Error: return errorComponent;
                case CompilerState.Empty: return emptyComponent;
                case CompilerState.Compiling: return compilingComponent;
                case CompilerState.Ready: return readyComponent;
            }
        }
    }

    Component {
        id: compilingComponent
        Item {
            Text {
                text: "Compiling"
            }
        }
    }
    Component {
        id: errorComponent
        Item {
            Text {
                text: "Error"
            }
        }
    }

    Component {
        id: emptyComponent
        Item {
            Text {
                text: "Empty"
            }
        }
    }
    Component {
        id: readyComponent
        RowLayout {
            width: 800
            height: 800
            TypeCheckerStateViewer {
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: {
                    checker.prepareStates()
                    return checker.getState(0)
                }
            }
            TypeCheckerStateViewer {
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: {
                    checker.prepareStates()
                    return checker.getState(1)
                }
            }
        }
    }
}
