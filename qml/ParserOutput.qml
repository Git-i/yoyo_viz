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
        delegate: TreeViewDelegate{

        }
    }
}
