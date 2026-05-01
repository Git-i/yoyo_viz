#pragma once
#include "compiler_state.hpp"
#include "qabstractitemmodel.h"
#include "qnamespace.h"
#include "qqmlintegration.h"
#include "qtmetamacros.h"
#include "root_statement.hpp"
#include <QAbstractItemModel>
#include <memory>
class ASTTreeViewModel : public QAbstractItemModel {
    Q_OBJECT
    Q_PROPERTY(CompilerState* state READ nullCompilerState WRITE setCompiler)
    QML_ELEMENT
public:
    ASTTreeViewModel(QObject* parent = nullptr): QAbstractItemModel(parent) {}
    Q_INVOKABLE void setCompiler(CompilerState* state);
    Q_INVOKABLE CompilerState* nullCompilerState() const;

    std::unique_ptr<RootStatement> root_stat;
    QModelIndex index(int row, int column, const QModelIndex& = {}) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    
    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    CompilerState* stt = nullptr;
    std::vector<std::unique_ptr<Yoyo::Statement>> empty_statement_set;
public slots:
    void compilerUpdated();
};
