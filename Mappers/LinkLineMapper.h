#pragma once
#include "Models/LinkLineModel.h"
#include <QJsonObject>
#include <optional>

class LinkLineMapper
{
public:
   static LinkLineModel* fromJson(const QJsonObject&);
   static QJsonObject toJson(const LinkLineModel*);

private:
   inline static QString minCardinality = "minCardinality";
   inline static QString maxCardinality = "maxCardinality";
};
