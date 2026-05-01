import QtQuick
import QtQuick.Shapes
import com.yoyoviz.cpp 1.0
Item {
    anchors.fill: parent
    id: root
    required property BorrowCheckerModel ck_model
    required property var edges
    required property var iR
    required property string graphName
    Repeater {
        model: root.edges
        delegate: Shape {
            id: edge
            required property var model
            preferredRendererType: Shape.CurveRenderer
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
                strokeColor: "#fff"
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
        model: root.iR
        delegate: Rectangle {
            required property var model
            required property int index
            id: parentRect
            x: model.posx - (width / 2)
            y: model.posy - (height / 2)

            width: model.width
            height: model.height

            radius: 5

            color: "#440000"

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
                            color: "#fff"
                        } 

                    }
                }
            }
                
        }
    }
}
