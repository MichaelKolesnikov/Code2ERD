#include "LinesConnectionsMapper.h"
#include "Mappers/PositionMapper.h"
#include <QJsonArray>

LinesConnectionsModel* LinesConnectionsMapper::fromJson(const QJsonObject& json)
{
   if (!json.contains(connectionsKey) || !json[connectionsKey].isArray())
      return nullptr;

   QJsonArray connectionsArray = json[connectionsKey].toArray();
   auto model = new LinesConnectionsModel();

   for (const auto& item : connectionsArray)
   {
      if (!item.isObject())
         return nullptr;

      QJsonObject connObj = item.toObject();

      if (!connObj.contains(lineIdKey) || !connObj[lineIdKey].isDouble() ||
         !connObj.contains(inKey) || !connObj[inKey].isBool() ||
         !connObj.contains(objIdKey) || !connObj[objIdKey].isDouble() ||
         !connObj.contains(pointKey) || !connObj[pointKey].isObject())
      {
         return nullptr;
      }

      auto lineId = connObj[lineIdKey].toString();
      bool in = connObj[inKey].toBool();
      auto objId = connObj[objIdKey].toString();

      auto pointOpt = PositionMapper::fromJson(connObj[pointKey].toObject());
      if (!pointOpt.has_value())
         return nullptr;

      model->set(lineId, in, objId, pointOpt.value());
   }
   return model;
}

QJsonObject LinesConnectionsMapper::toJson(const LinesConnectionsModel* model)
{
   QJsonArray connectionsArray;

   auto inConns = model->getInConnections();
   for (auto it = inConns.begin(); it != inConns.end(); ++it)
   {
      auto lineId = it.key();
      auto objId = it.value().first;
      QPointF point = it.value().second;

      QJsonObject connObj;
      connObj[lineIdKey] = lineId;
      connObj[inKey] = true;
      connObj[objIdKey] = objId;
      connObj[pointKey] = PositionMapper::toJson(point);

      connectionsArray.append(connObj);
   }

   auto outConns = model->getOutConnections();
   for (auto it = outConns.begin(); it != outConns.end(); ++it)
   {
      auto lineId = it.key();
      auto objId = it.value().first;
      QPointF point = it.value().second;

      QJsonObject connObj;
      connObj[lineIdKey] = lineId;
      connObj[inKey] = false;
      connObj[objIdKey] = objId;
      connObj[pointKey] = PositionMapper::toJson(point);

      connectionsArray.append(connObj);
   }

   QJsonObject result;
   result[connectionsKey] = connectionsArray;
   return result;
}
