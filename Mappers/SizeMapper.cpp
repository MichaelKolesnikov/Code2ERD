#include "SizeMapper.h"
#include "DoublePairMapper.h"
#include <QSizeF>

std::optional<QSizeF> SizeMapper::fromJson(const QJsonArray &jsonArray)
{
   auto opt = DoublePairMapper::fromJson(jsonArray);
   if (!opt)
   {
      return std::nullopt;
   }
   auto val = opt.value();
   return QSizeF(val.first, val.second);
}

QJsonArray SizeMapper::toJson(const QSizeF &size)
{
   return DoublePairMapper::toJson({size.width(), size.height()});
}
