#include "LineMapper.h"
#include "PositionMapper.h"
#include <QJsonArray>

std::optional<LineModel> LineMapper::fromJson(const QJsonObject &jsonObject)
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
      return std::nullopt;
   }
   auto jsonArray = jsonObject[moves].toArray();
   QVector<QPointF> lineMoves(jsonArray.size());
   int pos = 0;
   for (auto jsonObj : jsonArray)
   {
      if (!jsonObj.isObject())
      {
         return std::nullopt;
      }
      auto positionOpt = PositionMapper::fromJson(jsonObj.toObject());
      if (!positionOpt)
      {
         return std::nullopt;
      }
      lineMoves[pos++] = positionOpt.value();
   }
   return LineModel(jsonObject[id].toDouble(), lineMoves);
}

QJsonObject LineMapper::toJson(const LineModel &model)
{
   QJsonArray jsonArray;
   for (auto pos : model)
   {
      jsonArray.append(PositionMapper::toJson(pos));
   }
   return QJsonObject(
      {
         {id, model.id()},
         {position, jsonArray},
         {moves, jsonArray}
      }
   );
}
