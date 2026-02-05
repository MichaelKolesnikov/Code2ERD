#pragma once
#include "Models/LinkLineModel.h"
#include <QJsonObject>
#include <optional>

class LinkLineMapper
{
public:
   static std::optional<LinkLineModel> fromJson(const QJsonObject&);
   static QJsonObject toJson(const LinkLineModel&);

private:
   inline static QString entityId = "entityId";
   inline static QString linkId = "linkId";
   inline static QString minCardinality = "minCardinality";
   inline static QString maxCardinality = "maxCardinality";
};
