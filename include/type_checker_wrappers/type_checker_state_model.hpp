#pragma once

#include "constraint_list_model.hpp"
#include "qobject.h"
#include "qqmlintegration.h"
#include "qtmetamacros.h"
#include "compiler_state.hpp"
struct TypeCheckerStateModel : public QObject {
    Q_OBJECT
    QML_ELEMENT
public:
    TypeCheckerStateModel(QObject* parent, Yoyo::Info::RecordedTypeCheckerState* raw) :
        QObject(parent) , inner(raw) {}
    Q_INVOKABLE ConstraintListModel* getActiveConstraints();
    Q_INVOKABLE ConstraintListModel* getGeneratedConstraints();
private:
    Yoyo::Info::RecordedTypeCheckerState* inner;
};
