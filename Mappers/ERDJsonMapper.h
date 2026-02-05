#pragma once
#include "Models/ERDModel.h"
#include <QJsonObject>
#include <optional>

class ERDJsonConverter
{
public:
   static std::optional<ERDModel> fromJson(const QJsonObject&);
   static QJsonObject toJson(const ERDModel&);
private:
   inline static QString entities = "entities";
   inline static QString links = "links";
   inline static QString properties = "properties";
   inline static QString propertyLines = "propertyLines";
   inline static QString linkLines = "linkLines";
};

