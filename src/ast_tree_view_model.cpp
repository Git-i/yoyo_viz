#include "ast_tree_view_model.hpp"
#include "qabstractitemmodel.h"
#include "qobject.h"
#include "root_statement.hpp"
#include <algorithm>
#include <utility>

struct GetChildrenVisitor {
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::Expression*) {
        return {};
    }
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::Statement*) {
        return {};
    }
    std::vector<Yoyo::ASTNode*> get_children(Yoyo::ASTNode* node,  RootStatement* root_stat) {
        std::vector<Yoyo::ASTNode*> children;
        if(auto as_expr = dynamic_cast< Yoyo::Expression*>(node)) {
            children = std::visit(*this, as_expr->toVariant());
        } else if (node == root_stat) {
            std::ranges::transform(*root_stat->tree, std::back_inserter(children), [](auto& n) {
                return n.get();
            });
        } else if (auto as_stat = dynamic_cast< Yoyo::Statement*>(node)) {
            children = std::visit(*this, as_stat->toVariant());
        }
        std::unreachable();
    }
    Yoyo::ASTNode* get_nth_child(size_t n,  Yoyo::ASTNode* node,  RootStatement* root_stat) {
        auto children = get_children(node, root_stat);
        if (children.size() > n) return children[n];
        else return nullptr;
    }
};
// this is never dereferenced i promise
void ASTTreeViewModel::setCompiler(CompilerState* state) {
    QObject::connect(state, &CompilerState::statusChanged, this, &ASTTreeViewModel::compilerUpdated);
    this->root_stat = std::make_unique<RootStatement>(&state->syntax_tree);
}
void ASTTreeViewModel::compilerUpdated() {
    beginResetModel();
    endResetModel();
}
QModelIndex ASTTreeViewModel::index(int row, int column, const QModelIndex& parent) const {
    if(!hasIndex(row, column, parent)) return {};
    Yoyo::ASTNode* parent_node = parent.isValid() ? static_cast<Yoyo::ASTNode*>(parent.internalPointer())
        : root_stat.get();

    if(auto child = GetChildrenVisitor{}.get_nth_child(row, parent_node, root_stat.get())) {
        return createIndex(row, column, child);
    }
    return {};
}
QModelIndex ASTTreeViewModel::parent(const QModelIndex& index) const {
    if (!index.isValid()) return {};
    auto child = static_cast<Yoyo::ASTNode*>(index.internalPointer());

    auto parent = child->parent;
    if(parent == nullptr) {
        // there are 2 cases where null is valid
        // 1 - we are root node
        if (child == root_stat.get()) return {};
        auto it = std::ranges::find_if(*root_stat->tree, [child](auto& ptr) {
            return child == ptr.get();
        });
        // 2 - is if its a direct child of root
        if (it != root_stat->tree->end()) {
            return createIndex(index.row() - 1, index.column(), root_stat.get());
        }
        std::unreachable();
    }
    return createIndex(index.row() - 1, index.column(), parent);
}

int ASTTreeViewModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid())
        return GetChildrenVisitor{}.get_children(static_cast<Yoyo::ASTNode*>(parent.internalPointer()), root_stat.get()).size();
    return root_stat->tree->size();
}

int ASTTreeViewModel::columnCount(const QModelIndex& parent) const {
    return 1;
}
