#pragma once
#include <optional>
#include <QJsonArray>

class PositionMapper
{
public:
   static std::optional<QPointF> fromJson(const QJsonArray&);
   static QJsonArray toJson(const QPointF&);
};

