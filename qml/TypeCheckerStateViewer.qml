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
            Layout.preferredWidth: parent.width / 2
            Text {
                text: "Active Constraints"
                font.pointSize: 12
                color: '#4c4f69'
            }
            ConstraintListViewer {
                width: parent.width
                height: 500
                listModel: root.model.getActiveConstraints()
            }

        }
        Column {
            Layout.preferredWidth: parent.width / 2
            Text {
                text: "Generated Constraints"
                font.pointSize: 12
                color: '#4c4f69'
            }
            ConstraintListViewer {
                width: parent.width
                height: 500
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
