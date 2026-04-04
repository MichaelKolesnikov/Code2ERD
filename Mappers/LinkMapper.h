#pragma once
#include "Models/LinkModel.h"
#include <QJsonObject>
#include <optional>

class LinkMapper
{
public:
   static LinkModel* fromJson(const QJsonObject&);
   static QJsonObject toJson(const LinkModel*);
};
