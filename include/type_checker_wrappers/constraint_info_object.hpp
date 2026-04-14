#pragma once
#include "info_aggregator.h"
#include <QObject>
#include "qabstractitemmodel.h"
#include "qobject.h"
#include "qtmetamacros.h"
struct ConstraintInfoWrapper: public QObject {
    Q_OBJECT
public:
    ConstraintInfoWrapper(QObject* parent = nullptr) : QObject(parent) {}
    ConstraintInfoWrapper(QObject* parent, std::vector<Yoyo::Info::ConstraintInformation>* con, size_t idx)
        : QObject(parent), info(con), index(idx) {}
    Q_INVOKABLE QString description() const;
    Q_INVOKABLE QAbstractItemModel* details() const;
public:
    std::vector<Yoyo::Info::ConstraintInformation>* info;
    size_t index;
};
