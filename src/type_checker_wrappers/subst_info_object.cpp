#include "subst_info_object.hpp"
#include "qabstractitemmodel.h"
#include "qnamespace.h"
#include "qobject.h"
#include "qvariant.h"

QString SubstInfoWrapper::getType() const {
    return QString::fromStdString(info->at(index).type_var);
}

struct SubstTypesModel : public QAbstractListModel {
    SubstTypesModel(QObject* parent, const std::vector<Yoyo::Type>& types)
        : QAbstractListModel(parent) {
        this->types = types;
    }
    std::vector<Yoyo::Type> types;
    int rowCount(const QModelIndex&) const override { return types.size(); }
    QVariant data(const QModelIndex& index, int role) const override {
        if (role != Qt::DisplayRole) return {};
        return QString::fromStdString(types[index.row()].full_name());
    }
};

QAbstractItemModel* SubstInfoWrapper::getSubs() const {
    return new SubstTypesModel(nullptr, info->at(index).result_type);
}
