#pragma once

#include "qabstractitemmodel.h"
#include "qcontainerfwd.h"
#include "qnamespace.h"
#include "qobject.h"
#include "qqmlintegration.h"
#include "qtmetamacros.h"
#include "qvariant.h"
#include "subst_info_object.hpp"
#include <memory>
#include "info_aggregator.h" 
class SubstListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT
public:
    static constexpr int SubstRole = Qt::UserRole + 1;
    int rowCount(const QModelIndex& parent = QModelIndex{}) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    SubstListModel(QObject* parent, std::unordered_map<std::string, Yoyo::Info::SubstitutionInformation>& info);
private:
    std::vector<std::unique_ptr<SubstInfoWrapper>> sub_list;
};
