#pragma once
#include "DTO/IdNamePositionSizeTypeDTO.h"
#include <QJsonObject>
#include <optional>

class EntityMapper
{
public:
   static std::optional<IdNamePositionSizeTypeDTO> fromJson(const QJsonObject&);
   static QJsonObject toJson(const IdNamePositionSizeTypeDTO&);

private:
   inline static QString id = "id";
   inline static QString name = "name";
   inline static QString position = "position";
   inline static QString size = "size";
   inline static QString type = "type";
};

