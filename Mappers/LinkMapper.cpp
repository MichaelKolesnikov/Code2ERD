#include "LinkMapper.h"
#include "EntityMapper.h"

LinkModel* LinkMapper::fromJson(const QJsonObject &jsonObject)
{
   auto entityModelOpt = EntityMapper::fromJson(jsonObject);
   if (!entityModelOpt)
   {
      return nullptr;
   }
   auto entityModel = entityModelOpt;
   return new LinkModel(entityModel->id(), entityModel->name(), entityModel->position());
}

QJsonObject LinkMapper::toJson(const LinkModel *model)
{
   return EntityMapper::toJson(model);
}
