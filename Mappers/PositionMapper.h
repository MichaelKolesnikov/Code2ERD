#pragma once
#include <optional>
#include <QJsonObject>
#include <optional>

class PositionMapper
{
public:
   static std::optional<QPointF> fromJson(const QJsonObject&);
   static QJsonObject toJson(const QPointF&);

private:
   inline static QString x = "x";
   inline static QString y = "y";
};
