#pragma once
#include "DTO/LinkLineDTO.h"
#include <QJsonObject>
#include <optional>

class LinkLineMapper
{
public:
   static std::optional<LinkLineDTO> fromJson(const QJsonObject&);
   static QJsonObject toJson(const LinkLineDTO&);

private:
   inline static QString minCardinality = "minCardinality";
   inline static QString maxCardinality = "maxCardinality";
};
