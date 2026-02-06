#pragma once
#include "Models/LineModel.h"
#include <QJsonObject>
#include <optional>

class LineMapper
{
public:
   static std::optional<LineModel> fromJson(const QJsonObject&);
   static QJsonObject toJson(const LineModel&);
private:
   inline static QString id = "id";
   inline static QString position = "position";
   inline static QString moves = "moves";
};
