#include "constraint_list_model.hpp"
#include "constraint_info_object.hpp"
#include "qabstractitemmodel.h"
#include "qcontainerfwd.h"
#include "qobject.h"
#include "qvariant.h"
#include <iterator>
int ConstraintListModel::rowCount(const QModelIndex& parent) const {
    return static_cast<int>(con_list.size());
}
QVariant ConstraintListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= rowCount()) return {};
    if (role != ConstraintRole) return {};
    return QVariant::fromValue(con_list[index.row()].get());
}
ConstraintListModel::ConstraintListModel(QObject* parent, std::vector<Yoyo::Info::ConstraintInformation>& info) 
: QAbstractListModel(parent) {
    for (auto i : std::views::iota(0u, info.size())) {
        con_list.emplace_back(new ConstraintInfoWrapper(this, &info, i));
    }
}
QHash<int, QByteArray> ConstraintListModel::roleNames() const {
    return {
    { ConstraintListModel::ConstraintRole, "constraint" }
    };
}
