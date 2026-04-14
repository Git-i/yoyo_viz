#include "type_checker_wrappers/constraint_info_object.hpp"
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

QString ConstraintInfoWrapper::description() const {
    auto& self = info->at(index);
    return std::visit(ConstraintDescriptionVisitor{}, *self.constraint);
}

