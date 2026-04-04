#include "LineMapper.h"
#include "PositionMapper.h"
#include <QJsonArray>

LineModel* LineMapper::fromJson(const QJsonObject &jsonObject)
{
   bool isValid =
         jsonObject.contains(id) &&
         jsonObject[id].isDouble() &&
         jsonObject.contains(position) &&
         jsonObject[position].isObject() &&
         jsonObject.contains(moves) &&
         jsonObject[moves].isArray();
   if (!isValid)
   {
      return nullptr;
   }
   auto jsonArray = jsonObject[moves].toArray();
   QVector<QPointF> lineMoves(jsonArray.size());
   int pos = 0;
   for (auto jsonObj : jsonArray)
   {
      if (!jsonObj.isObject())
      {
         return nullptr;
      }
      auto positionOpt = PositionMapper::fromJson(jsonObj.toObject());
      if (!positionOpt)
      {
         return nullptr;
      }
      lineMoves[pos++] = positionOpt.value();
   }
   return new LineModel(jsonObject[id].toString(), lineMoves);
}

QJsonObject LineMapper::toJson(const LineModel *model)
{
   QJsonArray jsonArray;
   for (auto pos : *model)
   {
      jsonArray.append(PositionMapper::toJson(pos));
   }
   return QJsonObject(
      {
         {id, model->id()},
         {position, jsonArray},
         {moves, jsonArray}
      }
   );
}
