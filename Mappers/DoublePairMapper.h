#pragma once
#include <optional>
#include <QJsonArray>

class DoublePairMapper
{
public:
   static std::optional<QPair<double, double>> fromJson(const QJsonArray&);
   static QJsonArray toJson(const QPair<double, double>&);
};
