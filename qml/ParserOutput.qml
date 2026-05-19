import QtQuick
import QtQuick.Controls
import com.yoyoviz.cpp 1.0
Item {
    id: root
    anchors.fill: parent
    required property CompilerState state
    ASTTreeViewModel {
        id: treeModel
    }
    Component.onCompleted: {
        treeModel.setCompiler(state)
    }
    TreeView {
        anchors.fill: parent
        id: treeView
        model: treeModel
        selectionModel: ItemSelectionModel{}
        columnWidthProvider: function(column) {
            // In a single-column tree, make the column equal to the view width
            return treeView.width 
        }
        delegate: TreeViewDelegate{
        }
    }
}
