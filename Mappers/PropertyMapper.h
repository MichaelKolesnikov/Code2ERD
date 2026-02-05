#pragma once
#include "Models/PropertyModel.h"
#include <QJsonObject>
#include <optional>

class PropertyMapper
{
public:
   static std::optional<PropertyModel> fromJson(const QJsonObject&);
   static QJsonObject toJson(const PropertyModel&);
};

