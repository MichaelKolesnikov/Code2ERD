#include "DoublePairMapper.h"
#include <QPointF>

std::optional<QPair<double, double>> DoublePairMapper::fromJson(const QJsonArray &jsonObject)
{
   bool isCorrect = jsonObject.size() == 2 && jsonObject.at(0).isDouble() && jsonObject.at(1).isDouble();
   if (!isCorrect)
   {
      return std::nullopt;
   }
   return QPair<double, double>(jsonObject.at(0).toDouble(), jsonObject.at(1).toDouble());
}

QJsonArray DoublePairMapper::toJson(const QPair<double, double> &point)
{
   return QJsonArray({point.first, point.second});
}
