#include "PropertyMapper.h"
#include "EntityMapper.h"

PropertyModel* PropertyMapper::fromJson(const QJsonObject &jsonObject)
{
   auto entityModelOpt = EntityMapper::fromJson(jsonObject);
   if (!entityModelOpt)
   {
      return nullptr;
   }
   auto entityModel = entityModelOpt;
   return new PropertyModel(entityModel->id(), entityModel->name(), entityModel->position());
}

QJsonObject PropertyMapper::toJson(const PropertyModel *model)
{
   return EntityMapper::toJson(model);
}
