#pragma once
#include "compiler_state.hpp"
#include "qabstractitemmodel.h"
#include "qqmlintegration.h"
#include "qtmetamacros.h"
#include "root_statement.hpp"
#include <QAbstractItemModel>
#include <memory>
class ASTTreeViewModel : public QAbstractItemModel {
    Q_OBJECT
    QML_ELEMENT
public:
    ASTTreeViewModel(QObject* parent = nullptr): QAbstractItemModel(parent) {}
    Q_INVOKABLE void setCompiler(CompilerState* state);

    std::unique_ptr<RootStatement> root_stat;
    QModelIndex index(int row, int column, const QModelIndex& = {}) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    
    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;
public slots:
    void compilerUpdated();
};
