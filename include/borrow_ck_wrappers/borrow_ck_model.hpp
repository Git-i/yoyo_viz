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
    Q_PROPERTY(QAbstractItemModel* domainIR READ getDomainIR NOTIFY dataChanged)
    Q_PROPERTY(QAbstractItemModel* domainIREdges READ getDomainIREdges NOTIFY dataChanged)
    Q_PROPERTY(QAbstractItemModel* ssaIR READ getSsaIR NOTIFY dataChanged)
    Q_PROPERTY(QAbstractItemModel* ssaIREdges READ getSsaIREdges NOTIFY dataChanged)
    Q_PROPERTY(QAbstractItemModel* ptgraph READ getPtgraph NOTIFY dataChanged)
    Q_PROPERTY(QAbstractItemModel* ptgraphEdges READ getPtgraphEdges NOTIFY dataChanged)
    Q_PROPERTY(QAbstractItemModel* flowGraph READ getFlowGraph NOTIFY dataChanged)
    Q_PROPERTY(QAbstractItemModel* flowGraphEdges READ getFlowGraphEdges NOTIFY dataChanged)

public:
    BorrowCheckerModel(QObject* parent = nullptr): QObject(parent) {
        gvc = gvContext();
    }
    Q_INVOKABLE CompilerState* getCompiler() const;
    Q_INVOKABLE void setCompiler(CompilerState* state);
    Q_INVOKABLE QAbstractItemModel* getInitialIR() const;
    Q_INVOKABLE QAbstractItemModel* getInitialIREdges() const;
    Q_INVOKABLE QAbstractItemModel* getDomainIR() const;
    Q_INVOKABLE QAbstractItemModel* getDomainIREdges() const;
    Q_INVOKABLE QAbstractItemModel* getSsaIR() const;
    Q_INVOKABLE QAbstractItemModel* getSsaIREdges() const;
    Q_INVOKABLE QAbstractItemModel* getPtgraph() const;
    Q_INVOKABLE QAbstractItemModel* getPtgraphEdges() const;
    Q_INVOKABLE QAbstractItemModel* getFlowGraph() const;
    Q_INVOKABLE QAbstractItemModel* getFlowGraphEdges() const;
    Q_INVOKABLE void setNodeSize(QString graphName, int index, double width, double height);
    Q_INVOKABLE int getBBWidth(QString graphName) const;
    Q_INVOKABLE int getBBHeight(QString graphName) const;
private:
    void compilerStatusChanged();
    void initGraphFor(Agraph_t*& graph, Yoyo::BorrowChecker::BorrowCheckerFunction* func);
    void initPtGraph();
    void initFlowGraph();

    CompilerState* compiler = nullptr;
    Yoyo::Info::BorrowCheckerState* state = nullptr;
    bool dirty = false;
    QString functionName;
    GVC_t* gvc;

    Agraph_t* initialIR = nullptr;
    int initialIRLaidOut = 0; 
    std::unique_ptr<QAbstractItemModel> initialIRModel;
    std::unique_ptr<QAbstractItemModel> initialIREdgeModel;

    Agraph_t* domainIR = nullptr;
    int domainIRLaidOut = 0;
    std::unique_ptr<QAbstractItemModel> domainIRModel;
    std::unique_ptr<QAbstractItemModel> domainIREdgeModel;

    Agraph_t* ssaIR = nullptr;
    int ssaIRLaidOut = 0;
    std::unique_ptr<QAbstractItemModel> ssaIRModel;
    std::unique_ptr<QAbstractItemModel> ssaIREdgeModel;

    Agraph_t *ptgraph, *flowGraph;
    std::unique_ptr<QAbstractItemModel> ptgraphModel, flowGraphModel;
    std::unique_ptr<QAbstractItemModel> ptgraphEdgeModel, flowGraphEdgeModel;
signals:
    void dataChanged();
};
