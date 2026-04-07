#include "LinesConnectionsMapper.h"
#include "Mappers/DoublePairMapper.h"
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

      if (!connObj.contains(lineIdKey) || !connObj[lineIdKey].isString() ||
         !connObj.contains(inKey) || !connObj[inKey].isBool() ||
         !connObj.contains(objIdKey) || !connObj[objIdKey].isString() ||
         !connObj.contains(pointKey) || !connObj[pointKey].isArray())
      {
         return nullptr;
      }

      auto lineId = connObj[lineIdKey].toString();
      bool in = connObj[inKey].toBool();
      auto objId = connObj[objIdKey].toString();

      auto pointOpt = DoublePairMapper::fromJson(connObj[pointKey].toArray());
      if (!pointOpt.has_value())
         return nullptr;

      model->set(lineId, in, objId, {pointOpt.value().first, pointOpt.value().second});
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
      connObj[pointKey] = DoublePairMapper::toJson(QPair<double, double>(point.x(), point.y()));

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
      connObj[pointKey] = DoublePairMapper::toJson(QPair<double, double>(point.x(), point.y()));

      connectionsArray.append(connObj);
   }

   QJsonObject result;
   result[connectionsKey] = connectionsArray;
   return result;
}
