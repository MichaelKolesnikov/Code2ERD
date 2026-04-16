#include "LineMapper.h"
#include "PositionMapper.h"

std::optional<LineDTO> LineMapper::fromJson(const QJsonObject &jsonObject)
{
   auto nullValue = std::nullopt;
   bool isValid =
         jsonObject.contains(id) &&
         jsonObject[id].isString() &&
         jsonObject.contains(nodes) &&
         jsonObject[nodes].isArray();
   if (!isValid)
   {
      return nullValue;
   }
   auto jsonArray = jsonObject[nodes].toArray();
   QVector<QPointF> lineMoves(jsonArray.size());
   int pos = 0;
   for (auto jsonObj : jsonArray)
   {
      if (!jsonObj.isArray())
      {
         return nullValue;
      }
      auto positionOpt = PositionMapper::fromJson(jsonObj.toArray());
      if (!positionOpt)
      {
         return nullValue;
      }
      lineMoves[pos++] = positionOpt.value();
   }
   return LineDTO {jsonObject[id].toString(), lineMoves};
}

QJsonObject LineMapper::toJson(const LineDTO& dto)
{
   QJsonArray jsonArray;
   for (auto pos : dto.nodes)
   {
      jsonArray.append(PositionMapper::toJson(pos));
   }
   return QJsonObject(
      {
         {id, dto.id},
         {nodes, jsonArray}
      }
   );
}
