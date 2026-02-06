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
   auto linePosOpt = PositionMapper::fromJson(jsonObject[position].toObject());
   if (!linePosOpt.has_value())
   {
      return std::nullopt;
   }
   auto jsonArray = jsonObject[moves].toArray();
   QVector<int> lineMoves(jsonArray.size());
   int pos = 0;
   for (auto jsonObj : jsonArray)
   {
      if (!jsonObj.isDouble())
      {
         return std::nullopt;
      }
      lineMoves[pos++] = jsonObj.toDouble();
   }
   return LineModel(jsonObject[id].toDouble(), linePosOpt.value(), lineMoves);
}

QJsonObject LineMapper::toJson(const LineModel &model)
{
   QJsonArray jsonArray;
   for (auto pos : model)
   {
      jsonArray.append(pos);
   }
   return QJsonObject(
      {
         {id, model.id()},
         {position, jsonArray},
         {moves, jsonArray}
      }
   );
}
