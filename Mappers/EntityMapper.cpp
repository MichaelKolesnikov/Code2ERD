#include "EntityMapper.h"
#include "PositionMapper.h"

std::optional<EntityModel> EntityMapper::fromJson(const QJsonObject &jsonObject)
{
   bool isValid =
         jsonObject.contains(id) &&
         jsonObject[id].isDouble() &&
         jsonObject.contains(name) &&
         jsonObject[name].isString() &&
         jsonObject.contains(position) &&
         jsonObject[position].isObject();
   if (!isValid)
   {
      return std::nullopt;
   }
   auto p = PositionMapper::fromJson(jsonObject[position].toObject());
   if (!p.has_value())
   {
      return std::nullopt;
   }
   return EntityModel(jsonObject[id].toDouble(), jsonObject[name].toString(), p.value());
}

QJsonObject EntityMapper::toJson(const EntityModel &model)
{
   return QJsonObject(
      {
         {id, model.id()},
         {name, model.name()},
         {position, PositionMapper::toJson(model.position())}
      }
   );
}
