import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls
import com.yoyoviz.cpp 1.0
Item {
    id: root
    anchors.fill: parent
    required property CompilerState compiler
    WheelHandler {
        id: wheel
        target: null
        acceptedModifiers: Qt.ControlModifier
        acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad
        onWheel: (event) => {
            const oldZoom = root.zoom
            const newZoom = Math.min(Math.max(zoom + event.angleDelta.y * 0.001, 0.1), 3.0)
            root.zoom = newZoom
            // flickable.contentX = (flickable.contentX + event.x) * (newZoom / oldZoom) - event.x
            // flickable.contentY = (flickable.contentY + event.y) * (newZoom / oldZoom) - event.y
            // flickable.contentWidth  = canvas.width * newZoom
            // flickable.contentHeight = canvas.height * newZoom
        }
    }
    BorrowCheckerModel {
        id: bck_model
        compiler: root.compiler
        functionName: "source::main"
    }
    ColumnLayout {
        anchors.fill: parent
        TabBar {
            id: tab
            Layout.fillWidth: true
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
                Layout.fillWidth: true; Layout.fillHeight: true
                id: canvas
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
                ck_model: bck_model
                graphName: "auxPTG"
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
                ck_model: bck_model
                graphName: "finalPTG"
                edges: bck_model.flowGraphEdges
                nodes: bck_model.flowGraph
            }
        }
    }
}
