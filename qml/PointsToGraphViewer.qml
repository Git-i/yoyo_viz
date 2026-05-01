import QtQuick
import QtQuick.Shapes
Item {
    id: root
    anchors.fill: parent
    required property var edges
    required property var nodes
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
            }
        }
    }

    Repeater {
        model: root.nodes
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
            Text {
                anchors.centerIn: parent
                text: parentRect.model.display
                color: "#fff"
            }
        }
    }
}
