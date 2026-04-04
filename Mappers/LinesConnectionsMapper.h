#pragma once
#include "Models/LinesConnectionsModel.h"
#include <QJsonObject>
#include <optional>

class LinesConnectionsMapper
{
public:
    static LinesConnectionsModel* fromJson(const QJsonObject& json);
    static QJsonObject toJson(const LinesConnectionsModel* model);

private:
    inline static QString connectionsKey = "connections";
    inline static QString lineIdKey = "lineId";
    inline static QString inKey = "in";
    inline static QString objIdKey = "objId";
    inline static QString pointKey = "point";
};
