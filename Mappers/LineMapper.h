#pragma once
#include "Models/LineModel.h"
#include <QJsonObject>
#include <optional>

class LineMapper
{
public:
   static LineModel* fromJson(const QJsonObject&);
   static QJsonObject toJson(const LineModel*);
private:
   inline static QString id = "id";
   inline static QString nodes = "nodes";
};
