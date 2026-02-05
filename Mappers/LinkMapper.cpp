#include "LinkMapper.h"
#include "EntityMapper.h"

std::optional<LinkModel> LinkMapper::fromJson(const QJsonObject &jsonObject)
{
   auto entityModelOpt = EntityMapper::fromJson(jsonObject);
   if (!entityModelOpt.has_value())
   {
      return std::nullopt;
   }
   auto entityModel = entityModelOpt.value();
   return LinkModel(entityModel.id(), entityModel.name(), entityModel.position());
}

QJsonObject LinkMapper::toJson(const LinkModel &model)
{
   return EntityMapper::toJson(model);
}
