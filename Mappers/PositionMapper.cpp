#include "PositionMapper.h"
#include "DoublePairMapper.h"
#include <QPointF>

std::optional<QPointF> PositionMapper::fromJson(const QJsonArray &jsonArray)
{
   auto opt = DoublePairMapper::fromJson(jsonArray);
   if (!opt)
   {
      return std::nullopt;
   }
   auto val = opt.value();
   return QPointF(val.first, val.second);
}

QJsonArray PositionMapper::toJson(const QPointF &point)
{
   return DoublePairMapper::toJson({point.x(), point.y()});
}
