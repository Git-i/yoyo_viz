#pragma once 
#include "constraint_info_object.hpp"
#include "qqmlintegration.h"
#include <QAbstractListModel>
#include <info_aggregator.h>
struct ConstraintListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT
public:
    static constexpr int ConstraintRole = Qt::UserRole + 1;
    int rowCount(const QModelIndex& parent = QModelIndex{}) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
private:
    std::vector<ConstraintInfoWrapper> con_list;
};
