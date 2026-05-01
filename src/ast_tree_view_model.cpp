#include "ast_tree_view_model.hpp"
#include "compiler_state.hpp"
#include "qabstractitemmodel.h"
#include "qnamespace.h"
#include "qobject.h"
#include "root_statement.hpp"
#include <algorithm>
#include <memory>
#include <print>
#include <cinttypes>
#include <format>
#include <iterator>
#include <utility>
#include <vector>

struct GetChildrenVisitor {
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::Expression*) {
        return {};
    }
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::BlockExpression* expr) {
        std::vector<Yoyo::ASTNode*> result;
        std::ranges::transform(expr->statements, std::back_inserter(result), [](auto& in) { return in.get(); });
        if(expr->expr) result.push_back(expr->expr.get());
        return result;
    }
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::BinaryOperation* expr) {
        return {
            expr->lhs.get(),
            expr->rhs.get()
        };
    }
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::PrefixOperation* expr) {
        return { expr->operand.get() };
    }
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::CallOperation* expr) {
        std::vector<Yoyo::ASTNode*> result;
        result.push_back(expr->callee.get());
        std::ranges::transform(expr->arguments, std::back_inserter(result), [](auto& in) { return in.get(); });
        return result;
    }
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::ForStatement* stat) {
        return {
            stat->iterable.get(),
            stat->body.get()
        };
    }
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::ClassDeclaration* stat) {
        std::vector<Yoyo::ASTNode*> result;
        std::ranges::transform(stat->stats, std::back_inserter(result), [](auto& in) { return in.get(); });
        return result;
    }
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::WhileStatement* stat) {
        return {
            stat->condition.get(),
            stat->body.get()
        };
    }
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::ReturnStatement* stat) {
        return stat->expression ? std::vector<Yoyo::ASTNode*>{ stat->expression.get() } : std::vector<Yoyo::ASTNode*>{};
    }
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::FunctionDeclaration* stat) {
        return {
            stat->body.get()
        };
    }
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::VariableDeclaration* stat) {
        return stat->initializer ?
            std::vector<Yoyo::ASTNode*>{stat->initializer.get()} :
            std::vector<Yoyo::ASTNode*>{};
    }
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::ExpressionStatement* stat) {
        return {
            stat->expression.get()
        };
    }
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::ModuleImport* stat) { return {}; }
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::EnumDeclaration* stats) {
        std::vector<Yoyo::ASTNode*> result;
        std::ranges::transform(stats->stats, std::back_inserter(result), [](auto& in) { return in.get(); });
        return result;
    }
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::OperatorOverload* stat) {
        return {
            stat->body.get()
        };
    }
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::WithStatement* stat) {
        std::unreachable();
    }
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::GenericFunctionDeclaration* stat) {
        return {
            stat->body.get()
        };
    }
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::GenericAliasDeclaration* stat) {
        return {};
    }
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::AliasDeclaration* stat) { return {}; }
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::InterfaceDeclaration* stat) {
        std::vector<Yoyo::ASTNode*> result;
        std::ranges::transform(stat->methods, std::back_inserter(result), [](auto& in) { return in.get(); });
        return result;
    }
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::GenericInterfaceDeclaration* stat) {
        std::vector<Yoyo::ASTNode*> result;
        std::ranges::transform(stat->methods, std::back_inserter(result), [](auto& in) { return in.get(); });
        return result;
    }
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::GenericClassDeclaration* stat) {
        std::vector<Yoyo::ASTNode*> result;
        std::ranges::transform(stat->stats, std::back_inserter(result), [](auto& in) { return in.get(); });
        return result;
    }
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::BreakStatement* stat) { return {}; }
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::ContinueStatement*) { return {}; }
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::CImportDeclaration*) { return {}; }
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::ConstantDeclaration* stat) {
        return {
            stat->expr.get()
        };
    }
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::UnionDeclaration* stat) {
        std::vector<Yoyo::ASTNode*> result;
        std::ranges::transform(stat->sub_stats, std::back_inserter(result), [](auto& in) { return in.get(); });
        return result;
    }
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::MacroDeclaration*) {
        std::unreachable();
    }
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::UsingStatement*) { 
        std::unreachable();
    }
    std::vector<Yoyo::ASTNode*> operator()(Yoyo::Statement*) {
        std::unreachable();
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
        return children;
    }
    Yoyo::ASTNode* get_nth_child(size_t n,  Yoyo::ASTNode* node,  RootStatement* root_stat) {
        auto children = get_children(node, root_stat);
        if (children.size() > n) return children[n];
        else return nullptr;
    }
};
struct GetDataVisitor {
    std::string operator()(Yoyo::Expression*) {
        return "unlimplemented";
    }
    std::string operator()(Yoyo::BlockExpression*) {
        return "block";
    }
    std::string operator()(Yoyo::IntegerLiteral* lit) {
        return std::format("int: {}", lit->text);
    }
    std::string operator()(Yoyo::Statement*) { return "unlimplemented statement"; }
    std::string operator()(Yoyo::ReturnStatement* stat) {
        auto added_string = stat->expression ? "" : " void";
        return std::string("return") + added_string;
    }
    std::string operator()(Yoyo::ExpressionStatement* stat) {
        return "expression statement";
    }
    std::string operator()(Yoyo::FunctionDeclaration* stat) {
        return std::format("function decl: {}", stat->name);
    }
    QVariant get_data(Yoyo::ASTNode* node, RootStatement* root_stat) {
        if (auto as_expr = dynamic_cast<Yoyo::Expression*>(node)) {
            auto content = std::visit(*this, as_expr->toVariant());
            return QString::fromStdString(std::format("[expr:{}] {}", as_expr->evaluated_type.full_name(), content));
        } else if (node == root_stat) {
            return "root node";
        } else if (auto as_stat = dynamic_cast<Yoyo::Statement*>(node)) {
            return QString::fromStdString(std::visit(*this, as_stat->toVariant()));
        }
        std::unreachable();
    }
};
void ASTTreeViewModel::setCompiler(CompilerState* state) {
    QObject::connect(state, &CompilerState::statusChanged, this, &ASTTreeViewModel::compilerUpdated);
    stt = state;
    this->root_stat = std::make_unique<RootStatement>(&state->syntax_tree);
    compilerUpdated();
}
void ASTTreeViewModel::compilerUpdated() {
    if(stt->status != CompilerState::Ready)
        this->root_stat->tree = &empty_statement_set;
    else
        this->root_stat->tree = &stt->syntax_tree;
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
int node_row(Yoyo::ASTNode* node, RootStatement* root) {
    auto parent = node->parent;
    if(parent == nullptr) {
        if (node == root) return 0;
        auto it = std::ranges::find_if(*root->tree, [node](auto& ptr) {
            return node == ptr.get();
        });
        if (it != root->tree->end()) {
            return std::distance(root->tree->begin(), it);
        } else std::unreachable();
    }
    auto siblings = GetChildrenVisitor{}.get_children(parent, root);
    auto it = std::ranges::find(siblings, node);
    if(it != siblings.end())
        return std::distance(siblings.begin(), it);
    std::unreachable();
}
QModelIndex ASTTreeViewModel::parent(const QModelIndex& index) const {
    if (!index.isValid()) return {};
    auto child = static_cast<Yoyo::ASTNode*>(index.internalPointer());
    if (child == nullptr) std::unreachable();
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
            return QModelIndex{};
            // return createIndex(0, 0, root_stat.get());
        }
        std::println("{} {} {}", (void*)child, index.row(), index.column());
        std::unreachable();
        return {};
    }
    return createIndex(node_row(parent, root_stat.get()), index.column(), parent);
}

int ASTTreeViewModel::rowCount(const QModelIndex& parent) const {
    if (parent.column() > 0) return 0;
    if (parent.isValid())
        return GetChildrenVisitor{}.get_children(static_cast<Yoyo::ASTNode*>(parent.internalPointer()), root_stat.get()).size();
    return root_stat ? root_stat->tree->size() : 0;
}

int ASTTreeViewModel::columnCount(const QModelIndex& parent) const {
    return 1;
}

QVariant ASTTreeViewModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole)
        return {};

    auto node = static_cast<Yoyo::ASTNode*>(index.internalPointer());
    return GetDataVisitor{}.get_data(node, root_stat.get());
}
CompilerState* ASTTreeViewModel::nullCompilerState() const { 
    return nullptr;
}
