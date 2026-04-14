#pragma once
#include "qobject.h"
#include "qtmetamacros.h"
#include "info_aggregator.h"
#include <unordered_map>
class SubstInfoWrapper : public QObject 
{
    Q_OBJECT
public:
   SubstInfoWrapper(QObject* parent, std::unordered_map<std::string, Yoyo::Info::SubstitutionInformation>* subs, std::string idx)
        : QObject(parent), info(subs), index(idx) {}
private:
    std::unordered_map<std::string, Yoyo::Info::SubstitutionInformation>* info;
    std::string index;
};
