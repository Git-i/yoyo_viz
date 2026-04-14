#pragma once
#include "info_aggregator.h"
#include <QObject>
#include "qtmetamacros.h"
struct ConstraintInfoWrapper: public QObject {
    Q_OBJECT
public:
    ConstraintInfoWrapper(QObject* parent = nullptr) : QObject(parent) {}
    ConstraintInfoWrapper(std::vector<Yoyo::Info::ConstraintInformation>* con, size_t idx)
        : QObject(nullptr), info(con), index(idx) {}
    Q_INVOKABLE QString description() const;
public:
    std::vector<Yoyo::Info::ConstraintInformation>* info;
    size_t index;
};
