import QtQuick

Item {
    id: root
    property ConstraintListModel listModel
    ListView {
        id: constraints
        model: root.listModel
        delegate: Text {
            required property var model
            text: model.constraint.description()
        }
    }
}
