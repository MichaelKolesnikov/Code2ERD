#pragma once
#include "Models/EntityModel.h"
#include <QJsonObject>
#include <optional>

class EntityMapper
{
public:
   static std::optional<EntityModel> fromJson(const QJsonObject&);
   static QJsonObject toJson(const EntityModel&);

private:
   inline static QString id = "id";
   inline static QString name = "name";
   inline static QString position = "position";
};

