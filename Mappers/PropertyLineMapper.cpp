#include "PropertyLineMapper.h"
#include "LineMapper.h"

std::optional<PropertyLineModel> PropertyLineMapper::fromJson(const QJsonObject &jsonObject)
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
      jsonObject.contains(propertyId) &&
      jsonObject[propertyId].isDouble();
   if (!isValid)
   {
      return std::nullopt;
   }
   return PropertyLineModel(lineModel.id(), lineModel.position(), lineModel, jsonObject[entityId].toDouble(), jsonObject[propertyId].toDouble());
}

QJsonObject PropertyLineMapper::toJson(const PropertyLineModel &model)
{
   auto jsonObject = LineMapper::toJson(model);
   jsonObject[entityId] = model.entityId();
   jsonObject[propertyId] = model.propertyId();
   return jsonObject;
}
