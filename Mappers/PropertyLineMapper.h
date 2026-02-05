#pragma once
#include "Models/PropertyLineModel.h"
#include <QJsonObject>
#include <optional>

class PropertyLineMapper
{
public:
   static std::optional<PropertyLineModel> fromJson(const QJsonObject&);
   static QJsonObject toJson(const PropertyLineModel&);
private:
   inline static QString entityId = "entityId";
   inline static QString propertyId = "propertyId";
};
