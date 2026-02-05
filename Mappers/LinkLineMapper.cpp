#include "LinkLineMapper.h"
#include "LineMapper.h"

std::optional<LinkLineModel> LinkLineMapper::fromJson(const QJsonObject &jsonObject)
{
   auto lineOpt = LineMapper::fromJson(jsonObject);
   if (!lineOpt.has_value())
   {
      return std::nullopt;
   }
   auto lineModel = lineOpt.value();
   bool isValid =
      jsonObject.contains(entityId) &&
      jsonObject[entityId].isDouble() &&
      jsonObject.contains(linkId) &&
      jsonObject[linkId].isDouble() &&
      jsonObject.contains(minCardinality) &&
      jsonObject[minCardinality].isString() &&
      jsonObject.contains(maxCardinality) &&
      jsonObject[maxCardinality].isString();
   if (!isValid)
   {
      return std::nullopt;
   }
   return LinkLineModel(
      lineModel.id(),
      lineModel,
      jsonObject[entityId].toDouble(),
      jsonObject[linkId].toDouble(),
      jsonObject[minCardinality].toString(),
      jsonObject[maxCardinality].toString()
   );
}

QJsonObject LinkLineMapper::toJson(const LinkLineModel &model)
{
   auto jsonObject = LineMapper::toJson(model);
   jsonObject[entityId] = model.entityId();
   jsonObject[linkId] = model.linkId();
   jsonObject[minCardinality] = model.minCardinality();
   jsonObject[maxCardinality] = model.maxCardinality();
   return jsonObject;
}
