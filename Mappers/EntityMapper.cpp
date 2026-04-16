#include "EntityMapper.h"
#include "PositionMapper.h"
#include "SizeMapper.h"

std::optional<IdNamePositionSizeTypeDTO> EntityMapper::fromJson(const QJsonObject &jsonObject)
{
   auto nullValue = std::nullopt;
   bool isValid =
         jsonObject.contains(id) &&
         jsonObject[id].isString() &&
         jsonObject.contains(name) &&
         jsonObject[name].isString() &&
         jsonObject.contains(position) &&
         jsonObject[position].isArray() &&
         jsonObject.contains(size) &&
         jsonObject[size].isArray() &&
         jsonObject.contains(type) &&
         jsonObject[type].isDouble();
   if (!isValid)
   {
      return nullValue;
   }
   auto p_ = PositionMapper::fromJson(jsonObject[position].toArray());
   if (!p_.has_value())
   {
      return nullValue;
   }
   auto siz_ = SizeMapper::fromJson(jsonObject[size].toArray());
   if (!siz_.has_value())
   {
      return nullValue;
   }
   int typeInt = jsonObject[type].toDouble();
   if (typeInt < 0 || typeInt > 2)
   {
      typeInt = 0;
   }
   auto type = static_cast<IdNamePositionSizeTypeDTO::Type >(typeInt);
   return IdNamePositionSizeTypeDTO {{jsonObject[id].toString()}, jsonObject[name].toString(), p_.value(), siz_.value(), type};
}

QJsonObject EntityMapper::toJson(const IdNamePositionSizeTypeDTO& dto)
{
   return QJsonObject(
      {
         {id, dto.id},
         {name, dto.name},
         {position, PositionMapper::toJson(dto.position)},
         {size, SizeMapper::toJson(dto.size)},
         {type, (int)dto.type}
      }
   );
}
