#pragma once

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
private:
    Yoyo::Info::RecordedTypeCheckerState* inner;
};
