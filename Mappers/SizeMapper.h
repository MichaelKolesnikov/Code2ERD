#pragma once
#include <QJsonArray>
#include <optional>

class SizeMapper
{
public:
   static std::optional<QSizeF> fromJson(const QJsonArray&);
   static QJsonArray toJson(const QSizeF&);
};

