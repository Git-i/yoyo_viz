#include "cgraph.h"
#include "compiler_state.hpp"
#include "gvc.h"
#include "qabstractitemmodel.h"
#include "qqmlintegration.h"
#include "qtmetamacros.h"
#include <QObject>
struct BorrowCheckerModel : public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString functionName MEMBER functionName)
    Q_PROPERTY(CompilerState* compiler READ getCompiler WRITE setCompiler)
    Q_PROPERTY(QAbstractItemModel* initialIR READ getInitialIR NOTIFY dataChanged)
    Q_PROPERTY(QAbstractItemModel* initialIREdges READ getInitialIREdges NOTIFY dataChanged)
public:
    BorrowCheckerModel(QObject* parent = nullptr): QObject(parent) {
        gvc = gvContext();
    }
    Q_INVOKABLE CompilerState* getCompiler() const;
    Q_INVOKABLE void setCompiler(CompilerState* state);
    Q_INVOKABLE QAbstractItemModel* getInitialIR() const;
    Q_INVOKABLE QAbstractItemModel* getInitialIREdges() const;
    Q_INVOKABLE void setNodeSize(QString graphName, int index, double width, double height);
private:
    void compilerStatusChanged();
    void initGraphFor(Agraph_t*& graph, Yoyo::BorrowChecker::BorrowCheckerFunction* func);

    CompilerState* compiler = nullptr;
    Yoyo::Info::BorrowCheckerState* state = nullptr;
    bool dirty = false;
    QString functionName;
    GVC_t* gvc;

    Agraph_t* initialIR = nullptr;
    int initialIRLaidOut = 0; 
    QAbstractItemModel* initialIRModel = nullptr;
    QAbstractItemModel* initialIREdgeModel = nullptr;
signals:
    void dataChanged();
};
