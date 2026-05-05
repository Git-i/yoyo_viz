import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
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
        Item {
            width: parent.width; height: parent.height
            id: readyRoot
            property int numStates: {
                checker.prepareStates()
                return checker.numStates()
            }
            ColumnLayout {
                id: readyLayout
                anchors.fill: parent
                Button {
                    text: "Next Step"
                    onClicked: {
                        const currentNum = listModel.get(0).idx
                        listModel.clear()
                        listModel.append({"idx": currentNum + 1});
                        listModel.append({"idx": currentNum + 2});
                    }
                }
                ListModel {
                    id: listModel
                    ListElement{ idx: 0 }
                    ListElement{ idx: 1 }
                }
                ListView {
                    Layout.fillHeight: true; Layout.fillWidth: true
                    orientation: ListView.Horizontal
                    model: listModel
                    spacing: 10
                    delegate: Item {
                        required property var model
                        id: delg
                        width: ListView.view.width / 2
                        height: ListView.view.height
                        TypeCheckerStateViewer {
                            anchors.fill: parent
                            visible: delg.model.idx < (checker.numStates() - 1)
                            model: visible ? checker.getState(delg.model.idx) : null
                        }
                    }
                }
            }
        }
        // ListView {
        //     width: 800; height: 800
        //     orientation: ListView.Horizontal
        //     snapMode: ListView.SnapOneItem
        //     model: {
        //         checker.prepareStates()
        //         return checker.numStates()
        //     }
        //     delegate: RowLayout {
        //         width: 800; height: 800
        //         TypeCheckerStateViewer {
        //             Layout.fillWidth: true
        //             Layout.fillHeight: true
        //             model: checker.getState(index)
        //         }
        //         TypeCheckerStateViewer {
        //             Layout.fillWidth: true
        //             Layout.fillHeight: true
        //             visible: index < (checker.numStates() - 1)
        //             model: visible ? checker.getState(index + 1) : null
        //         }
        //     }
        // }
    }
}
