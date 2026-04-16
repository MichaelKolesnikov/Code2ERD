#pragma once
#include "Models/ERDModel.h"
#include <QJsonObject>

class ERDMapper
{
public:
   static ERDModel* fromJson(const QJsonObject&);
   static QJsonObject toJson(const ERDModel* const);
private:
   inline static QString entities = "entities";
   inline static QString propertyLines = "propertyLines";
   inline static QString linkLines = "linkLines";
   inline static QString linesConnections = "linesConnections";
};

