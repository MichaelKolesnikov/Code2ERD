#include "PositionMapper.h"
#include <QPointF>

std::optional<QPointF> PositionMapper::fromJson(const QJsonObject &jsonObject)
{
   bool isCorrect = jsonObject.contains(x) && jsonObject[x].isDouble() && jsonObject.contains(y) && jsonObject[y].isDouble();
   if (!isCorrect)
   {
      return std::nullopt;
   }
   return QPointF(jsonObject[x].toDouble(), jsonObject[y].toDouble());
}

QJsonObject PositionMapper::toJson(const QPointF &point)
{
   return QJsonObject(
      {
         {x, point.x()},
         {y, point.y()}
      }
   );
}
