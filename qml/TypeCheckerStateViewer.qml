import QtQuick
import QtQuick.Layouts
import com.yoyoviz.cpp 1.0
Item {
    id: root
    required property TypeCheckerStateModel model
    GridLayout {
        columns: 2
        width: parent.width
        Column {
            Layout.fillWidth: true
            Text {
                text: "Active Constraints"
                font.pointSize: 12
                color: '#fff'
            }
            ConstraintListViewer {
                width: parent.width / 4
                height: 100
                listModel: root.model.getActiveConstraints()
            }

        }
        Column {
            Layout.fillWidth: true
            Text {
                text: "Generated Constraints"
                font.pointSize: 12
                color: '#fff'
            }
            ConstraintListViewer {
                width: parent.width
                height: 100
                listModel: root.model.getGeneratedConstraints()
            }
        }
        SubstitutionListViewer {
            Layout.columnSpan: 2
            Layout.fillWidth: true
            height: 700
            listModel: root.model.getSubstitutions()
        }
    }
    
}
