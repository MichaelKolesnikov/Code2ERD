#include "LineMapper.h"
#include "DoublePairMapper.h"
#include <QJsonArray>

LineModel* LineMapper::fromJson(const QJsonObject &jsonObject)
{
   bool isValid =
         jsonObject.contains(id) &&
         jsonObject[id].isString() &&
         jsonObject.contains(nodes) &&
         jsonObject[nodes].isArray();
   if (!isValid)
   {
      return nullptr;
   }
   auto jsonArray = jsonObject[nodes].toArray();
   QVector<QPointF> lineMoves(jsonArray.size());
   int pos = 0;
   for (auto jsonObj : jsonArray)
   {
      if (!jsonObj.isArray())
      {
         return nullptr;
      }
      auto positionOpt = DoublePairMapper::fromJson(jsonObj.toArray());
      if (!positionOpt)
      {
         return nullptr;
      }
      lineMoves[pos++] = QPointF(positionOpt.value().first, positionOpt.value().second);
   }
   return new LineModel(jsonObject[id].toString(), lineMoves);
}

QJsonObject LineMapper::toJson(const LineModel *model)
{
   QJsonArray jsonArray;
   for (auto poss : *model)
   {
      auto pos = QPair<double, double>(poss.x(), poss.y());
      jsonArray.append(DoublePairMapper::toJson(pos));
   }
   return QJsonObject(
      {
         {id, model->id()},
         {nodes, jsonArray}
      }
   );
}
