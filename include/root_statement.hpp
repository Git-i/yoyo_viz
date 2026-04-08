#pragma once
#include "statement.h"
#include <memory>

class RootStatement : public Yoyo::Statement {
public:
    Yoyo::StatementVariant toVariant() override {
        return static_cast<Yoyo::FunctionDeclaration*>(nullptr);
    }
    std::vector<std::unique_ptr<Yoyo::Statement>>* tree;
    explicit RootStatement(decltype(RootStatement::tree) tree)
        : tree(tree) {}
    RootStatement() = default;
};
