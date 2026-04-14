#include "constraint_list_model.hpp"
#include "constraint_info_object.hpp"
#include "qabstractitemmodel.h"
#include "qcontainerfwd.h"
#include "qvariant.h"
int ConstraintListModel::rowCount(const QModelIndex& parent) const {
    return static_cast<int>(con_list.size());
}
QVariant ConstraintListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= rowCount()) return {};
    if (role != ConstraintRole) return {};
    return QVariant::fromValue(&con_list[index.row()]);
}
QHash<int, QByteArray> ConstraintListModel::roleNames() const {
    return {
    { ConstraintListModel::ConstraintRole, "constraint" }
    };
}
