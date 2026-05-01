import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls
import com.yoyoviz.cpp 1.0
Item {
    id: root
    required property CompilerState compiler
    BorrowCheckerModel {
        id: bck_model
        compiler: root.compiler
        functionName: "source::main"
    }
    ColumnLayout {
        anchors.fill: parent
        TabBar {
            id: tab
            TabButton {
                text: "Initial IR"
            }
            TabButton {
                text: "IR With Domain Varaibles"
            }
            TabButton {
                text: "Aux Points-To Graph"
            }
            TabButton {
                text: "SSA IR"
            }
            TabButton {
                text: "Final Points-To Graph"
            }
        }
        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: tab.currentIndex
            BorrowCheckerIRViewer {
                ck_model: bck_model
                edges: bck_model.initialIREdges
                iR: bck_model.initialIR
                graphName: "initialIR"
            }
            BorrowCheckerIRViewer {
                ck_model: bck_model
                edges: bck_model.domainIREdges
                iR: bck_model.domainIR
                graphName: "domainIR"
            }
            PointsToGraphViewer {
                edges: bck_model.ptgraphEdges
                nodes: bck_model.ptgraph
            }
            BorrowCheckerIRViewer {
                ck_model: bck_model
                edges: bck_model.ssaIREdges
                iR: bck_model.ssaIR
                graphName: "ssaIR"
            }
            PointsToGraphViewer {
                edges: bck_model.flowGraphEdges
                nodes: bck_model.flowGraph
            }
        }
    }
}
