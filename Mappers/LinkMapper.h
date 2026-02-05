#pragma once
#include "Models/LinkModel.h"
#include <QJsonObject>
#include <optional>

class LinkMapper
{
public:
   static std::optional<LinkModel> fromJson(const QJsonObject&);
   static QJsonObject toJson(const LinkModel&);
};
