import QtQuick
import com.yoyoviz.cpp 1.0
Item {
    id: root
    required property TypeCheckerStateModel model
    Grid {
        columns: 2
        width: parent.width
        Column {
            width: parent.width / 2
            Text {
                text: "Active Constraints"
                font.pointSize: 12
                color: '#fff'
            }
            ConstraintListViewer {
                width: parent.width
                height: 700
                listModel: root.model.getActiveConstraints()
            }

        }
        Column {
            width: parent.width / 2
            Text {
                text: "Generated Constraints"
                font.pointSize: 12
                color: '#fff'
            }
            ConstraintListViewer {
                width: parent.width
                height: 700
                listModel: root.model.getGeneratedConstraints()
            }
        }
    }
    
}
