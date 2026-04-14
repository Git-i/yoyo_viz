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
    Q_INVOKABLE void setCompiler(CompilerState* state);
    Q_INVOKABLE TypeCheckerStateModel* getState(int index);
private:
    void prepareStates();
    QString functionName;
    std::vector<std::unique_ptr<TypeCheckerStateModel>> states;
    std::vector<Yoyo::Info::RecordedTypeCheckerState> raw_states_buffer;
    CompilerState* compiler;
};
