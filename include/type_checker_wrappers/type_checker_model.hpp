#pragma once

#include "qobject.h"
#include "qqmlintegration.h"
#include "qtmetamacros.h"
#include "compiler_state.hpp"
#include "type_checker_state_model.hpp"
#include <memory>
struct TypeCheckerModel : public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString functionName MEMBER functionName)
public:
    TypeCheckerModel(QObject* parent = nullptr): QObject(parent) {}
    Q_INVOKABLE void setCompiler(CompilerState* state);
    Q_INVOKABLE TypeCheckerStateModel* getState(int index);
    Q_INVOKABLE void prepareStates();
    Q_INVOKABLE int numStates();
private:
    bool dirty = false;
    void compilerStatusChanged();
    QString functionName;
    std::vector<std::unique_ptr<TypeCheckerStateModel>> states;
    std::vector<Yoyo::Info::RecordedTypeCheckerState> raw_states_buffer;
    CompilerState* compiler;
};
