#include "PropertyMapper.h"
#include "EntityMapper.h"

std::optional<PropertyModel> PropertyMapper::fromJson(const QJsonObject &jsonObject)
{
   auto entityModelOpt = EntityMapper::fromJson(jsonObject);
   if (!entityModelOpt.has_value())
   {
      return std::nullopt;
   }
   auto entityModel = entityModelOpt.value();
   return PropertyModel(entityModel.id(), entityModel.name(), entityModel.position());
}

QJsonObject PropertyMapper::toJson(const PropertyModel &model)
{
   return EntityMapper::toJson(model);
}
