#pragma once
#include "DTO/LineDTO.h"
#include <QJsonObject>
#include <optional>

class LineMapper
{
public:
   static std::optional<LineDTO> fromJson(const QJsonObject&);
   static QJsonObject toJson(const LineDTO&);
private:
   inline static QString id = "id";
   inline static QString nodes = "nodes";
};
