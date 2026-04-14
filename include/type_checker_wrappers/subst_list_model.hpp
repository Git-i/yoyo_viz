#pragma once

#include "qabstractitemmodel.h"
#include "qnamespace.h"
#include "qqmlintegration.h"
#include "qtmetamacros.h"
class SubstListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT
public:
    static constexpr int SubstRole = Qt::UserRole + 1;
};
