#include "compiler_state.hpp"
#include "qobject.h"
#include "qqmlintegration.h"
#include "qtmetamacros.h"
class ErrorViewer : public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(CompilerState* state READ nullCompilerState WRITE setCompiler)
public:
    ErrorViewer(QObject* parent = nullptr): QObject(parent) {}
    Q_INVOKABLE void setCompiler(CompilerState* state);
    Q_INVOKABLE CompilerState* nullCompilerState() const;

    Q_INVOKABLE int numErrors();
    Q_INVOKABLE QString errorAt(int i);
    CompilerState* stt = nullptr;
};
