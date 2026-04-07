#pragma once
#include "Models/EntityModel.h"
#include <QJsonObject>

class EntityMapper
{
public:
   static EntityModel* fromJson(const QJsonObject&);
   static QJsonObject toJson(const EntityModel*);

private:
   inline static QString id = "id";
   inline static QString name = "name";
   inline static QString position = "position";
   inline static QString size = "size";
};

