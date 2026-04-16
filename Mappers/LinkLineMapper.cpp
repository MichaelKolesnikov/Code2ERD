#include "LinkLineMapper.h"
#include "LineMapper.h"

std::optional<LinkLineDTO> LinkLineMapper::fromJson(const QJsonObject &jsonObject)
{
   auto nullValue = std::nullopt;
   auto lineOpt = LineMapper::fromJson(jsonObject);
   if (!lineOpt)
   {
      return nullValue;
   }
   auto lineDTO = lineOpt.value();
   bool isValid =
      jsonObject.contains(minCardinality) &&
      jsonObject[minCardinality].isString();
   if (!isValid)
   {
      return nullValue;
   }
   isValid =
      jsonObject.contains(maxCardinality) &&
      jsonObject[maxCardinality].isString();
   if (!isValid)
   {
      return nullValue;
   }
   return LinkLineDTO {
      lineDTO.id,
      lineDTO.nodes,
      jsonObject[minCardinality].toString(),
      jsonObject[maxCardinality].toString()
   };
}

QJsonObject LinkLineMapper::toJson(const LinkLineDTO& dto)
{
   auto jsonObject = LineMapper::toJson({dto.id, dto.nodes});
   jsonObject[minCardinality] = dto.minCardinality;
   jsonObject[maxCardinality] = dto.maxCardinality;
   return jsonObject;
}
