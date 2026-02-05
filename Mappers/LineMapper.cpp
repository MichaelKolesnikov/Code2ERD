#include "LineMapper.h"
#include "PositionMapper.h"
#include <QJsonArray>

std::optional<LineModel> LineMapper::fromJson(const QJsonObject &jsonObject)
{
   bool isValid =
         jsonObject.contains(id) &&
         jsonObject[id].isDouble() &&
         jsonObject.contains(nodes) &&
         jsonObject[nodes].isArray();
   if (!isValid)
   {
      return std::nullopt;
   }
   auto jsonArray = jsonObject[nodes].toArray();
   QVector<QPointF> lineNodes(jsonArray.size());
   int pos = 0;
   for (auto jsonObj : jsonArray)
   {
      if (!jsonObj.isObject())
      {
         return std::nullopt;
      }
      if (auto position = PositionMapper::fromJson(jsonObj.toObject()))
      {
         lineNodes[pos++] = position.value();
      }
      else
      {
         return std::nullopt;
      }
   }
   return LineModel(jsonObject[id].toDouble(), lineNodes);
}

QJsonObject LineMapper::toJson(const LineModel &model)
{
   QJsonArray jsonNodesArray;
   for (auto pos : model)
   {
      jsonNodesArray.append(PositionMapper::toJson(pos));
   }
   return QJsonObject(
      {
         {id, model.id()},
         {nodes, jsonNodesArray}
      }
   );
}
