#include "type_checker_wrappers/subst_list_model.hpp"
#include "qabstractitemmodel.h"
#include "subst_info_object.hpp"
int SubstListModel::rowCount(const QModelIndex&) const {
    return static_cast<int>(sub_list.size());
} 
SubstListModel::SubstListModel(QObject* parent, std::unordered_map<std::string, Yoyo::Info::SubstitutionInformation>& info)
: QAbstractListModel(parent) {
    for (auto&[name, subst] : info) {
        sub_list.emplace_back(new SubstInfoWrapper(this, &info, name));
    }
}
QHash<int, QByteArray> SubstListModel::roleNames() const {
    return {
    { SubstListModel::SubstRole, "substitution" }
    };
}
QVariant SubstListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= rowCount()) return {};
    if (role != SubstRole) return {};
    return QVariant::fromValue(sub_list[index.row()].get());
}
