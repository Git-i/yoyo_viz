#include <QObject>
#include "qqmlintegration.h"
#include "qquicktextdocument.h"
#include "qtmetamacros.h"
#include "yvm/yvm_engine.h"
#include "statement.h"
#include "type_checker.h"
#include <thread>
#include <QObject>
class CompilerState : public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(Status status MEMBER status NOTIFY statusChanged)
    Q_PROPERTY(QQuickTextDocument* document MEMBER document)
public:
    explicit CompilerState(QObject* parent = nullptr)
        : QObject(parent), status(Empty) {}
    enum Status {
        Empty, Compiling, Ready, Error
    };
    Q_ENUM(Status)
    Q_INVOKABLE void compile();
    ~CompilerState();
    void setStatus(Status s);
private:
    QQuickTextDocument* document;
    Yoyo::YVMEngine engine;
    std::atomic<Status> status;
    std::vector<std::unique_ptr<Yoyo::Statement>> syntax_tree;
    Yoyo::CompilationOutput::ModuleCompilationResult output;
    std::thread compilation_thread;
signals:
    void statusChanged();
};
