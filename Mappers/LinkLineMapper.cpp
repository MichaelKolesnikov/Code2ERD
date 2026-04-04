#include "LinkLineMapper.h"
#include "LineMapper.h"

LinkLineModel* LinkLineMapper::fromJson(const QJsonObject &jsonObject)
{
   auto lineOpt = LineMapper::fromJson(jsonObject);
   if (!lineOpt)
   {
      return nullptr;
   }
   auto lineModel = lineOpt;
   bool isValid =
      jsonObject.contains(minCardinality) &&
      jsonObject[minCardinality].isString();
   if (!isValid)
   {
      return nullptr;
   }
   isValid =
      jsonObject.contains(maxCardinality) &&
      jsonObject[maxCardinality].isString();
   if (!isValid)
   {
      return nullptr;
   }
   return new LinkLineModel(
      lineModel->id(),
      *lineModel,
      jsonObject[minCardinality].toString(),
      jsonObject[maxCardinality].toString()
   );
}

QJsonObject LinkLineMapper::toJson(const LinkLineModel *model)
{
   auto jsonObject = LineMapper::toJson(model);
   jsonObject[minCardinality] = model->minCardinality();
   jsonObject[maxCardinality] = model->maxCardinality();
   return jsonObject;
}
