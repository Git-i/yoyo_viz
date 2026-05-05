#include "borrow_ck_wrappers/borrow_ck_model.hpp"
#include "cgraph.h"
#include "compiler_state.hpp"
#include "gvc.h"
#include "qabstractitemmodel.h"
#include "qnamespace.h"
#include "qobject.h"
#include "qpoint.h"
#include "qstringview.h"
#include "qvariant.h"
#include "types.h"
#include <algorithm>
#include <csignal>
#include <iterator>
CompilerState* BorrowCheckerModel::getCompiler() const {
    return compiler;
}
void BorrowCheckerModel::setCompiler(CompilerState* cmp) {
    compiler = cmp;
    QObject::connect(compiler, &CompilerState::statusChanged, this, &BorrowCheckerModel::compilerStatusChanged);
}
static double graph_h(Agraph_t* graph) {
    return GD_bb(graph).UR.y;
}
struct InstructionsModel : public QAbstractListModel {
    InstructionsModel(QObject* parent, Yoyo::BorrowChecker::BasicBlock* blk)
        : QAbstractListModel(parent), block(blk) {}
    int rowCount(const QModelIndex&) const override {
        return block->instructions.size();
    }
    QVariant data(const QModelIndex& index, int role) const override {
        if (role == Qt::DisplayRole) {
            return QString::fromStdString(block->instructions[index.row()]->to_string());
        }
        return {};
    }
    Yoyo::BorrowChecker::BasicBlock* block;
};
struct BCEdgesModel : public QAbstractListModel {
    static constexpr int Points = Qt::UserRole + 1;


    BCEdgesModel(QObject* parent, Agraph_t* graph) : QAbstractListModel(parent), graph(graph) {
        for(auto n = agfstnode(graph); n; n = agnxtnode(graph, n)) {
            for (auto e = agfstout(graph, n); e; e = agnxtout(graph, e)) {
                edges.push_back(e);
            }
        }
    }

    Agraph_t* graph;
    std::vector<Agedge_t*> edges;
    QHash<int, QByteArray> roleNames() const override {
        return {
            { Points, "points" }
        };
    }
    int rowCount(const QModelIndex& parent) const override {
        return edges.size();
    }
    QVariant data(const QModelIndex& index, int role) const override {
        if (role == Points) {
            QList<QVariant> ret_val;
            auto spline = ED_spl(edges[index.row()]);
            if (spline == nullptr) {
                ret_val.emplace_back(QPointF(0.f, 0.f));
                ret_val.emplace_back(QPointF(0.f, 0.f));
                ret_val.emplace_back(QPointF(0.f, 0.f));
                return ret_val;
            }
            for (auto i : std::views::iota(0uz, spline->size)) {
                auto& bez = spline->list[i];
                for(auto bez_point_idx : std::views::iota(0uz, bez.size)) {
                    auto& point = bez.list[bez_point_idx];
                    ret_val.emplace_back(QPointF(point.x, graph_h(graph) - point.y));
                }
            }
            return ret_val;
        }
        return {};
    }
    void refresh() {
        beginResetModel();
        endResetModel();
    }
};
struct BCNodesModel : public QAbstractListModel {
    static constexpr int PosX = Qt::UserRole + 1;
    static constexpr int PosY = Qt::UserRole + 2;
    static constexpr int Width = Qt::UserRole + 3;
    static constexpr int Height = Qt::UserRole + 4;

    Agraph_t* graph;
    std::vector<Agnode_t*> nodes;

    BCNodesModel(QObject* parent,  Agraph_t* graph): QAbstractListModel(parent), graph(graph) {
        for (auto n = agfstnode(graph); n; n = agnxtnode(graph, n)) {
            nodes.push_back(n);
        }
    }
    int rowCount(const QModelIndex& parent = {}) const override {
        return nodes.size();
    }
    QVariant data(const QModelIndex& index, int role) const override {
        if (role == Qt::DisplayRole) {
            return QString(agnameof(nodes[index.row()]));
        }
        if (role == PosX) {
            return ND_coord(nodes[index.row()]).x;
        }
        if (role == PosY) {
            return graph_h(graph) - ND_coord(nodes[index.row()]).y;
        }
        if (role == Width) {
            return ND_width(nodes[index.row()]) * 72.0;
        }
        if (role == Height) {
            return ND_height(nodes[index.row()]) * 72.0;
        }
        return {};
    }
    QHash<int, QByteArray> roleNames() const override {
        return {
            { PosX, "posx" },
            { PosY, "posy" },
            { Width, "width" },
            { Height, "height" },
            { Qt::DisplayRole, "display" } 
        };
    }
};
struct BCFunctionModel : public QAbstractListModel {
    static constexpr int PosX = Qt::UserRole + 1;
    static constexpr int PosY = Qt::UserRole + 2;
    static constexpr int Width = Qt::UserRole + 3;
    static constexpr int Height = Qt::UserRole + 4;
    static constexpr int Name = Qt::UserRole + 5;
    static constexpr int IsLaidOut = Qt::UserRole + 6;
    static constexpr int Instructions = Qt::UserRole + 7;

    Agraph_t* graph;
    std::vector<Agnode_t*> nodes;
    std::vector<std::unique_ptr<InstructionsModel>> node_inst_models;
    Yoyo::BorrowChecker::BorrowCheckerFunction* func;
    GVC_t* gvc;

    QHash<int, QByteArray> roleNames() const override {
        return {
            { PosX, "posx" },
            { PosY, "posy" },
            { Width, "width" },
            { Height, "height" },
            { Name, "name" },
            { IsLaidOut, "isLaidOut" },
            { Instructions, "instructions" }
        };
    }
    int rowCount(const QModelIndex&) const override {
        return nodes.size();
    }
    QVariant data(const QModelIndex& index, int role) const override {
        if (role == PosX) {
            return ND_coord(nodes[index.row()]).x;
        }
        if (role == PosY) {
            return graph_h(graph) - ND_coord(nodes[index.row()]).y;
        }
        if (role == Width) {
            return ND_width(nodes[index.row()]) * 72.0;
        }
        if (role == Height) {
            return ND_height(nodes[index.row()]) * 72.0;
        }
        if (role == Name) {
            return QString(agnameof(nodes[index.row()]));
        }
        if (role == IsLaidOut) {
            auto w = ND_width(nodes[index.row()]);
            return !(w >= (10000 - 1)); // I subtracted 1 because I don't trust floating points
        }
        if (role == Instructions) {
            return QVariant::fromValue(node_inst_models[index.row()].get());
        }
        return {};
    }
    void refresh() {
        beginResetModel();
        endResetModel();
    }
    BCFunctionModel(QObject* parent, Agraph_t* graph, decltype(BCFunctionModel::func) func)
        : QAbstractListModel(parent), graph(graph), func(func)
    {
        for (auto n = agfstnode(graph); n; n = agnxtnode(graph, n)) {
            nodes.push_back(n);
        }
        for (auto& blk : func->blocks) {
            node_inst_models.emplace_back(new InstructionsModel(this, blk.get()));
        }
    }
};
int BorrowCheckerModel::getBBWidth(QString graph) const {
    Agraph_t* gvgraph;
    if (graph == "initialIR") {
        gvgraph = initialIR;
    } else if (graph == "domainIR") {
        gvgraph = domainIR;
    } else if (graph == "ssaIR") {
        gvgraph = ssaIR;
    } else std::unreachable();
    if (!gvgraph) return 0;
    return GD_bb(gvgraph).UR.x;
}
int BorrowCheckerModel::getBBHeight(QString graph) const {
    Agraph_t* gvgraph;
    if (graph == "initialIR") {
        gvgraph = initialIR;
    } else if (graph == "domainIR") {
        gvgraph = domainIR;
    } else if (graph == "ssaIR") {
        gvgraph = ssaIR;
    } else std::unreachable();

    if (!gvgraph) return 0;
    return GD_bb(gvgraph).UR.y;
}

void BorrowCheckerModel::setNodeSize(QString graph, int index, double w, double h) {
    if (graph == "initialIR") {
        auto node = agnode(initialIR, state->initial_IR->blocks[index]->debug_name.data(), false);
        agsafeset(node, "width", std::to_string(w).data(), "");
        agsafeset(node, "height", std::to_string(h).data(), "");
        if (++initialIRLaidOut == agnnodes(initialIR)) {
            initialIRLaidOut = 0;
            gvLayout(gvc, initialIR, "dot");
            reinterpret_cast<BCFunctionModel*>(initialIRModel.get())->refresh();
            reinterpret_cast<BCEdgesModel*>(initialIREdgeModel.get())->refresh();
        }
    }
    if (graph == "domainIR") {
        auto node = agnode(domainIR, state->domain_vars_IR->blocks[index]->debug_name.data(), false);
        agsafeset(node, "width", std::to_string(w).data(), "");
        agsafeset(node, "height", std::to_string(h).data(), "");
        if (++domainIRLaidOut == agnnodes(domainIR)) {
            domainIRLaidOut = 0;
            gvLayout(gvc, domainIR, "dot");
            reinterpret_cast<BCFunctionModel*>(domainIRModel.get())->refresh();
            reinterpret_cast<BCEdgesModel*>(domainIREdgeModel.get())->refresh();
        }
    }
    if (graph == "ssaIR") {
        auto node = agnode(ssaIR, state->ssa_IR->blocks[index]->debug_name.data(), false);
        agsafeset(node, "width", std::to_string(w).data(), "");
        agsafeset(node, "height", std::to_string(h).data(), "");
        if (++ssaIRLaidOut == agnnodes(ssaIR)) {
            ssaIRLaidOut = 0;
            gvLayout(gvc, ssaIR, "dot");
            reinterpret_cast<BCFunctionModel*>(ssaIRModel.get())->refresh();
            reinterpret_cast<BCEdgesModel*>(ssaIREdgeModel.get())->refresh();
        }
    }
}
struct EmptyListModel : public QAbstractListModel {
    QVariant data(const QModelIndex& , int ) const override { return {}; }
    int rowCount(const QModelIndex& ) const override { return 0; }
};
QAbstractItemModel* BorrowCheckerModel::getInitialIR() const {
    if (!state) {
        return new EmptyListModel();
    }
    return initialIRModel.get();
}
QAbstractItemModel* BorrowCheckerModel::getInitialIREdges() const {
    if (!state) {
        return new EmptyListModel();
    }
    return initialIREdgeModel.get();
}
QAbstractItemModel* BorrowCheckerModel::getDomainIR() const {
    if (!state) {
        return new EmptyListModel();
    }
    return domainIRModel.get();
}
QAbstractItemModel* BorrowCheckerModel::getDomainIREdges() const {
    if (!state) {
        return new EmptyListModel();
    }
    return domainIREdgeModel.get();
}
QAbstractItemModel* BorrowCheckerModel::getSsaIR() const {
    if (!state) {
        return new EmptyListModel();
    }
    return ssaIRModel.get();
}
QAbstractItemModel* BorrowCheckerModel::getSsaIREdges() const {
    if (!state) {
        return new EmptyListModel();
    }
    return ssaIREdgeModel.get();
}
QAbstractItemModel* BorrowCheckerModel::getPtgraphEdges() const {
    if (!state) {
        return new EmptyListModel();
    }
    return ptgraphEdgeModel.get();
}
QAbstractItemModel* BorrowCheckerModel::getPtgraph() const {
    if (!state) {
        return new EmptyListModel();
    }
    return ptgraphModel.get();
}
QAbstractItemModel* BorrowCheckerModel::getFlowGraph() const {
    if (!state) {
        return new EmptyListModel();
    }
    return flowGraphModel.get();
}
QAbstractItemModel* BorrowCheckerModel::getFlowGraphEdges() const {
    if (!state) {
        return new EmptyListModel();
    }
    return flowGraphEdgeModel.get();
}
void BorrowCheckerModel::compilerStatusChanged() {
    state = nullptr;
    if (initialIR) {
        agclose(initialIR);
        initialIR = nullptr;
    }
    if (domainIR) { 
        agclose(domainIR);
        domainIR = nullptr;
    }
    if (ssaIR) { 
        agclose(ssaIR);
        ssaIR = nullptr;
    }
    initialIRLaidOut = 0;
    domainIRLaidOut = 0;
    ssaIRLaidOut = 0;
    std::vector<QAbstractItemModel*> delete_this;
    if (compiler->status == CompilerState::Ready) {
        auto& info = compiler->output.compilation_info.function_info;
        auto fn_name = functionName.toStdString();
        if(info.contains(fn_name)) {
            state = &info.at(fn_name).bc_state;
            initGraphFor(initialIR, state->initial_IR.get());
            initGraphFor(domainIR, state->domain_vars_IR.get());
            initGraphFor(ssaIR, state->ssa_IR.get());
            initPtGraph(); initFlowGraph();
            delete_this.push_back(initialIRModel.release());
            delete_this.push_back(initialIREdgeModel.release());
            initialIRModel.reset(new BCFunctionModel(this, initialIR, state->initial_IR.get()));
            initialIREdgeModel.reset(new BCEdgesModel(this, initialIR));
            
            delete_this.push_back(domainIRModel.release());
            delete_this.push_back(domainIREdgeModel.release());
            domainIRModel.reset(new BCFunctionModel(this, domainIR, state->domain_vars_IR.get()));
            domainIREdgeModel.reset(new BCEdgesModel(this, domainIR));

            delete_this.push_back(ssaIRModel.release());
            delete_this.push_back(ssaIREdgeModel.release());
            ssaIRModel.reset(new BCFunctionModel(this, ssaIR, state->ssa_IR.get()));
            ssaIREdgeModel.reset(new BCEdgesModel(this, ssaIR));

            delete_this.push_back(ptgraphModel.release());
            delete_this.push_back(ptgraphEdgeModel.release());
            ptgraphModel.reset(new BCNodesModel(this, ptgraph));
            ptgraphEdgeModel.reset(new BCEdgesModel(this, ptgraph));

            delete_this.push_back(flowGraphModel.release());
            delete_this.push_back(flowGraphEdgeModel.release());
            flowGraphModel.reset(new BCNodesModel(this, flowGraph));
            flowGraphEdgeModel.reset(new BCEdgesModel(this, flowGraph));
        }
    }
    emit dataChanged();
    for (auto ptr : delete_this) if(ptr) delete ptr;
}

void BorrowCheckerModel::initFlowGraph() {
    flowGraph = agopen("G", Agdirected, nullptr);
    agsafeset(flowGraph, "rankdir", "LR", "");
    for (auto& [_, pointees] : state->final_ptg.domain_to_node) {
        for (auto& pointee : pointees) agnode(flowGraph, const_cast<char*>(pointee.data()), true);
    }
    for (auto& [ptr, pointees] : state->final_ptg.domain_to_node) {
        Agnode_t* node = nullptr;
        if (!pointees.empty()) node = agnode(flowGraph, const_cast<char*>(ptr.data()), true);
        for (auto& pointee : pointees) {
            agedge(flowGraph, node, agnode(flowGraph, const_cast<char*>(pointee.data()), false), "e1", true);
        }
    }
    gvLayout(gvc, flowGraph, "dot");
}

void BorrowCheckerModel::initPtGraph() {
    ptgraph = agopen("G", Agdirected, nullptr);
    agsafeset(ptgraph, "rankdir", "LR", "");
    for (auto& [_, pointees] : state->aux_ptg.pointee_pairs) {
        for (auto& pointee : pointees) agnode(ptgraph, const_cast<char*>(pointee.data()), true);
    }
    for (auto& [ptr, pointees] : state->aux_ptg.pointee_pairs) {
        Agnode_t* node = nullptr;
        if (!pointees.empty()) node = agnode(ptgraph, const_cast<char*>(ptr.data()), true);
        for (auto& pointee : pointees) {
            agedge(ptgraph, node, agnode(ptgraph, const_cast<char*>(pointee.data()), false), "e1", true);
        }
    }
    gvLayout(gvc, ptgraph, "dot");
}
void BorrowCheckerModel::initGraphFor(Agraph_t*& graph, Yoyo::BorrowChecker::BorrowCheckerFunction* func ) {
    graph = agopen("G", Agdirected, nullptr);
    for(auto& in : func->blocks) { 
        auto node = agnode(graph, in->debug_name.data(),  true);
        agsafeset(node, "fixedsize", "true", "");
        agsafeset(node, "width", "10000.0", "");
        agsafeset(node, "height", "0.0", "");
    }
    for (auto& block : func->blocks) {
        auto this_blk = agnode(graph, block->debug_name.data(), false);
        for(auto* child: block->instructions.back()->children()) {
            auto child_blk = agnode(graph, child->debug_name.data(), false);
            agedge(graph, this_blk, child_blk, "e1", true);
        }
    }
    gvLayout(gvc, graph, "nop");
}
