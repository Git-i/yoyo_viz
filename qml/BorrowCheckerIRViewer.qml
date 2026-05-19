import QtQuick
import QtQuick.Shapes
import com.yoyoviz.cpp 1.0
Item {
    id: root
    required property BorrowCheckerModel ck_model
    required property var edges
    required property var iR
    required property string graphName
    property real zoom: 1.0
    property real xoff: 0
    property real yoff: 0
    property real bbwidth: 0
    property real bbheight: 0

    function recalc_off() {
        root.xoff = (root.width - root.ck_model.getBBWidth(root.graphName)) / 2
        root.yoff = (root.height - root.ck_model.getBBHeight(root.graphName)) / 2
        root.bbwidth = root.ck_model.getBBWidth(root.graphName)
        root.bbheight = root.ck_model.getBBHeight(root.graphName)
    }
    onWidthChanged: { recalc_off() }
    onHeightChanged: { recalc_off() }
    clip: true
    Connections {
        target: root.edges
        function onModelReset() { root.recalc_off() }
    }
    Flickable {
        x: root.xoff * root.zoom
        y: root.yoff * root.zoom
        anchors.fill: parent
        contentHeight: root.bbheight * root.zoom
        contentWidth: root.bbwidth * root.zoom
        id: flickable
        // interactive: false
        WheelHandler {
            id: wheel
            target: null
            acceptedModifiers: Qt.ControlModifier
            acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad
            onWheel: (event) => {
                const oldZoom = root.zoom
                const newZoom = Math.min(Math.max(oldZoom + event.angleDelta.y * 0.001, 0.1), 4.0)
                root.zoom = newZoom
                // flickable.contentX = (flickable.contentX + event.x) * (newZoom / oldZoom) - event.x
                // flickable.contentY = (flickable.contentY + event.y) * (newZoom / oldZoom) - event.y
                flickable.contentWidth  = canvas.width * newZoom
                flickable.contentHeight = canvas.height * newZoom
                event.accepted = true
            }
        }

        Item {
            width: root.bbwidth
            height: root.bbheight
            scale: root.zoom
            transformOrigin: Item.Center
            Repeater {
                anchors.centerIn: parent
                model: root.edges
                delegate: Shape {
                    id: edge
                    required property var model
                    preferredRendererType: Shape.CurveRenderer
                    x: root.xoff
                    y: root.yoff
                    Instantiator {
                        model: (edge.model.points.length - 1) / 3
                        onObjectAdded: (index, object) => path.pathElements.push(object)
                        onObjectRemoved: (index, object) => { /* handle removal if model changes */ }

                        delegate: PathCubic {
                            required property int index
                            control1X: edge.model.points[index * 3 + 1].x
                            control1Y: edge.model.points[index * 3 + 1].y
                            control2X: edge.model.points[index * 3 + 2].x
                            control2Y: edge.model.points[index * 3 + 2].y
                            x: edge.model.points[index * 3 + 3].x
                            y: edge.model.points[index * 3 + 3].y
                        }
                    }
                    ShapePath {
                        strokeColor: "#4c4f69"
                        id: path
                        startX: edge.model.points[0].x
                        startY: edge.model.points[0].y
                        fillColor: "transparent"

                        // PathCubic {
                        //     control1X: edge.model.points[1].x; control1Y: edge.model.points[1].y
                        //     control2X: edge.model.points[2].x; control2Y: edge.model.points[2].y
                        //     x: edge.model.points[3].x; y: edge.model.points[3].y
                        // }
                    }
                }
            }

            Repeater {
                anchors.centerIn: parent
                model: root.iR
                delegate: Rectangle {
                    required property var model
                    required property int index
                    id: parentRect
                    x: model.posx - (width / 2) + root.xoff
                    y: model.posy - (height / 2) + root.yoff

                    width: model.width
                    height: model.height

                    radius: 5

                    color: "#eff1f5"
                    border.color: "#4c4f69"

                    Component.onCompleted: {
                        if (!model.isLaidOut) {
                            root.ck_model.setNodeSize(root.graphName, index, (childrenRect.width + 5) / 72.0, (childrenRect.height + 5) / 72.0)
                        }
                    }

                    Column {
                        anchors.centerIn: parent
                        Repeater {
                            model: parentRect.model.instructions
                            delegate: Item {
                                id: instruction
                                required property var model
                                width: childrenRect.width
                                height: childrenRect.height
                                Text {
                                    text: instruction.model.display
                                    font.family: "JetBrains Mono NF"
                                    color: "#4c4f69"
                                } 

                            }
                        }
                    }

                }
            }
        }
    }
}
