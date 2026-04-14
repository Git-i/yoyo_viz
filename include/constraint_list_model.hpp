#pragma once 
#include "constraint_info_object.hpp"
#include "qobject.h"
#include "qqmlintegration.h"
#include <QAbstractListModel>
#include <info_aggregator.h>
#include <memory>
struct ConstraintListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT
public:
    static constexpr int ConstraintRole = Qt::UserRole + 1;
    int rowCount(const QModelIndex& parent = QModelIndex{}) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    ConstraintListModel(QObject* parent, std::vector<Yoyo::Info::ConstraintInformation>& info);
private:
    std::vector<std::unique_ptr<ConstraintInfoWrapper>> con_list;
};
