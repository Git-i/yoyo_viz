#include "type_checker_wrappers/constraint_info_object.hpp"
#include "qabstractitemmodel.h"
#include "qnamespace.h"
#include "qvariant.h"
#include "type_checker.h"
using namespace Yoyo;
struct ConstraintDescriptionVisitor
{
    QString operator()(IsIntegerConstraint& ) {
        return "Is Inteteger";
    }
    QString operator()(CanStoreIntegerConstraint& ) {
        return "Can Store Inteteger"; 
    }
    QString operator()(IsFloatConstraint& ) {
        return "Is Float";
    }
    QString operator()(CanStoreRealConstraint& ) {
        return "Can Store Float";
    }
    QString operator()(ToStringConstraint& ) {
        return "Can Convert To String";
    }
    QString operator()(HasUnaryMinusConstraint& ) {
        return "Has Unary Minus (-)";
    }
    QString operator()(HasUnaryNotConstraint& ) {
        return "Has Unary Not (!)";
    }
    QString operator()(IsReferenceToConstraint& ) {
        return "Reference To";
    }
    QString operator()(IsNotReferenceConstraint& ) {
        return "Is Not Reference";
    }
    QString operator()(BinaryOperableConstraint& ) {
        return "Binary operable";
    }
    QString operator()(ComparableConstraint& ) {
        return "Comparison Result";
    }
    QString operator()(EqualConstraint& ) {
        return "Eqaul Constraint";
    }
    QString operator()(OwningConstraint& ) { return "should never happen (ownging con)";}
    QString operator()(IsInvocableConstraint& ) {
        return "Is Invocable";
    }
    QString operator()(ValidAsFunctionArgConstraint& ) {
        return "Is Function Arg";
    }
    QString operator()(IsReturnOfConstraint& ) {
        return "Is Retrun Of";
    }
    QString operator()(ImplInterfaceConstraint& con) {
        return "Implements Interface";
    }
    QString operator()(ExtractsToConstraint& con) {
        return "Extracts To";
    }
    QString operator()(RefExtractsToConstraint& con) {
        return "Ref Extracts To";
    }
    QString operator()(NonOwningConstraint& con) {
        return "should never happen (non-owning)";
    }
    QString operator()(AsConstraint& con) {
        return "As Constraint";
    }
    QString operator()(ConvertibleToConstraint& con) {
        return "Convertible To";
    }
    QString operator()(BinaryDotCompatibleConstraint& con) {
        return "Binary Dot Compatible";
    }
    QString operator()(ElseRefExtractsToConstraint& con) {
        return "Else Ref Extracts To";
    }
    QString operator()(ElseExtractsToConstraint& con) {
        return "Else Extracts To";
    }
    QString operator()(IfStatementConstraint& con) {
        return "If Statement Constraint";
    }
    QString operator()(EqualOrIsVoidConstraint& con) {
        return "Equal or Void Constraint";
    }
    QString operator()(IndexOperableConstraint& con) {
        return "Index Operable Constraint";
    }
    QString operator()(HasFieldConstraint& con) {
        return "Has Field Constraint";
    }
    QString operator()(AllFieldsConstraint& con) {
        return "All Fields Constraint";
    }
    QString operator()(BorrowResultConstraint& con) {
        return "Borrow Result Constraint";
    }
    QString operator()(BorrowResultMutConstraint& con) {
        return "Borrow result mut constraint";
    }
    QString operator()(IfEqualThenConstrain& con) {
        return "If Equal then apply constraint";
    }
};

struct ConstraintDetailsVisitor {
    using Return = std::vector<std::pair<QString, QString>>;

    QString type_to_qstr(const Type& type) {
        return QString::fromStdString(type.full_name());
    }
    Return operator()(IsIntegerConstraint& con) {
        return {
        {"Type", type_to_qstr(con.type)}
        };
    }
    Return operator()(CanStoreIntegerConstraint& con) {
        QString final;
        if(auto* as_uint = std::get_if<uint64_t>(&con.value)) {
            final.setNum(*as_uint);
        } else final.setNum(std::get<int64_t>(con.value));
        return {
            {"Type", type_to_qstr(con.type)},
            {"Value", std::move(final)}
        };
    }
    Return operator()(IsFloatConstraint& con) {
        return {
            {"Type", type_to_qstr(con.type)}
        };
    }
    Return operator()(CanStoreRealConstraint& con) {
        return {
            {"Type", type_to_qstr(con.type)},
            {"Value", QString::number(con.value)}
        };
    }
    Return operator()(ToStringConstraint& con) {
        return {
            {"Type", type_to_qstr(con.type)}
        };
    }
    Return operator()(HasUnaryMinusConstraint& con) {
        return {
            {"Type", type_to_qstr(con.type)},
            {"Returns", type_to_qstr(con.ret)}
        };
    }
    Return operator()(HasUnaryNotConstraint& con) {
        return {
            {"Type", type_to_qstr(con.type)},
            {"Returns", type_to_qstr(con.ret)}
        };
    }
    Return operator()(IsReferenceToConstraint& con) {
        return {
            {"Type", type_to_qstr(con.type)},
            {"Reference To", type_to_qstr(con.other)}
        };
    }
    Return operator()(IsNotReferenceConstraint& con) {
        return {
            {"Type", type_to_qstr(con.type)}
        };
    }
    Return operator()(BinaryOperableConstraint& con) {
        return {
            {"Left Type", type_to_qstr(con.left)},
            {"Operator", "not implemented"},
            {"Right Type", type_to_qstr(con.right)},
            {"Returns", type_to_qstr(con.result)}
        };
    }
    Return operator()(ComparableConstraint& con) {
        return {
            {"Type", type_to_qstr(con.type)}
        };
    }
    Return operator()(EqualConstraint& con) {
        return {
            {"Type 1", type_to_qstr(con.type1)},
            {"Type 2", type_to_qstr(con.type2)}
        };
    }
    Return operator()(OwningConstraint& ) { return {};}
    Return operator()(IsInvocableConstraint& con) {
        return {
            {"Type", type_to_qstr(con.type)}
        };
    }
    Return operator()(ValidAsFunctionArgConstraint& con) {
        return {
            {"Type", type_to_qstr(con.type)},
            {"Function", type_to_qstr(con.function)},
            {"Arg No.", QString::number(con.arg_no)}
        };
    }
    Return operator()(IsReturnOfConstraint& con) {
        return {
            {"Type", type_to_qstr(con.type)},
            {"Function", type_to_qstr(con.function)}
        };
    }
    Return operator()(ImplInterfaceConstraint& con) {
        return {
            {"Type", type_to_qstr(con.type)},
            {"Interface", type_to_qstr(con.interface)}
        };
    }
    Return operator()(ExtractsToConstraint& con) {
        return {
            {"Type", type_to_qstr(con.type)},
            {"Extracts To", type_to_qstr(con.dst)}
        };
    }
    Return operator()(RefExtractsToConstraint& con) {
        return {
            {"Type", type_to_qstr(con.type)},
            {"Extracts To", type_to_qstr(con.dst)}
        };
    }
    Return operator()(NonOwningConstraint& con) {
        return {};
    }
    Return operator()(AsConstraint& con) {
        return {
            {"Input", type_to_qstr(con.input_type)},
            {"Destination", type_to_qstr(con.dest)},
            {"Result", type_to_qstr(con.result)}
        };
    }
    Return operator()(ConvertibleToConstraint& con) {
        return {
            {"From", type_to_qstr(con.from)},
            {"To", type_to_qstr(con.to)}
        };
    }
    Return operator()(BinaryDotCompatibleConstraint& con) {
        // TODO: do the RHS
        return {
            {"Left", type_to_qstr(con.tp)},
            {"Result", type_to_qstr(con.result)}
        };
    }
    Return operator()(ElseRefExtractsToConstraint& con) {
        return {
            {"Type", type_to_qstr(con.type)},
            {"Destination", type_to_qstr(con.dst)},
            {"Is Mutable?", con.is_mut ? QString("true") : QString("false")}
        };
    }
    Return operator()(ElseExtractsToConstraint& con) {
        return {
            {"Type", type_to_qstr(con.type)},
            {"Destination", type_to_qstr(con.dst)}
        };
    }
    Return operator()(IfStatementConstraint& con) {
        return {
            {"Then Type", type_to_qstr(con.then_type)},
            {"Else Type", type_to_qstr(con.else_type)},
            {"Result", type_to_qstr(con.result)},
            {"Then Transfers Control", con.then_transfers_control ? QString("true") : QString("false")}
        };
    }
    Return operator()(EqualOrIsVoidConstraint& con) {
        return {
            {"Type 1 or void", type_to_qstr(con.type1)},
            {"Type 2", type_to_qstr(con.type2)}
        };
    }
    Return operator()(IndexOperableConstraint& con) {
        return {
            {"Expression", type_to_qstr(con.left)},
            {"Index", type_to_qstr(con.right)},
            {"Result", type_to_qstr(con.result)},
            {"Is Mutable", con.is_mutable ? QString("true") : QString("false")}
        };
    }
    Return operator()(HasFieldConstraint& con) {
        return {
            {"Subject", type_to_qstr(con.subject)},
            {"Field name", QString::fromStdString(con.field_name)},
            {"Result", type_to_qstr(con.result)}
        };
    }
    Return operator()(AllFieldsConstraint& con) {
        return {
            {"Subject", type_to_qstr(con.subject)}
        };
    }
    Return operator()(BorrowResultConstraint& con) {
        return {
            {"Subject", type_to_qstr(con.subject)},
            {"Result", type_to_qstr(con.result)}
        };
    }
    Return operator()(BorrowResultMutConstraint& con) {
        return {
            {"Subject", type_to_qstr(con.subject)},
            {"Result", type_to_qstr(con.result)}
        };
    }
    Return operator()(IfEqualThenConstrain& con) {
        return {
        {"Type 1", type_to_qstr(con.type1)},
        {"Type 2", type_to_qstr(con.type2)}
        };
    }
};

QString ConstraintInfoWrapper::description() const {
    auto& self = info->at(index);
    return std::visit(ConstraintDescriptionVisitor{}, *self.constraint);
}

struct DetailModel : public QAbstractTableModel {
    DetailModel(QObject* parent, TypeCheckerConstraint* con)
        : QAbstractTableModel(parent), inner(con) {
        table = std::visit(ConstraintDetailsVisitor{}, *con);
    }
    TypeCheckerConstraint* inner;
    std::vector<std::pair<QString, QString>> table;
public:
    int rowCount(const QModelIndex& ) const override {
        return table.size();
    }
    int columnCount(const QModelIndex& ) const override {
        return 2;
    }
    QVariant data(const QModelIndex& index, int role) const override {
        if (role != Qt::DisplayRole) return {};
        auto& row_entry = table[index.row()];
        if (index.column() == 0) return row_entry.first;
        else if (index.column() == 1) return row_entry.second;
        else return {};
    }
};

QAbstractItemModel* ConstraintInfoWrapper::details() const {
    return new DetailModel(nullptr, info->at(index).constraint.get());
}

