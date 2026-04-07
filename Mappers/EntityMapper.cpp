#include "EntityMapper.h"
#include "DoublePairMapper.h"
#include "QJsonArray"

EntityModel* EntityMapper::fromJson(const QJsonObject &jsonObject)
{
   bool isValid =
         jsonObject.contains(id) &&
         jsonObject[id].isString() &&
         jsonObject.contains(name) &&
         jsonObject[name].isString() &&
         jsonObject.contains(position) &&
         jsonObject[position].isArray() &&
         jsonObject.contains(size) &&
         jsonObject[size].isArray();
   if (!isValid)
   {
      return nullptr;
   }
   auto p_ = DoublePairMapper::fromJson(jsonObject[position].toArray());
   if (!p_.has_value())
   {
      return nullptr;
   }
   auto siz_ = DoublePairMapper::fromJson(jsonObject[size].toArray());
   if (!siz_.has_value())
   {
      return nullptr;
   }
   auto p = p_.value();
   auto siz = siz_.value();
   return new EntityModel(jsonObject[id].toString(), jsonObject[name].toString(), {p.first, p.second}, QSize(siz.first, siz.second));
}

QJsonObject EntityMapper::toJson(const EntityModel *model)
{
   auto pos = QPair<double, double>(model->position().x(), model->position().y());
   auto siz = QPair<double, double>(model->size().width(), model->size().height());
   return QJsonObject(
      {
         {id, model->id()},
         {name, model->name()},
         {position, DoublePairMapper::toJson(pos)},
         {size, DoublePairMapper::toJson(siz)}
      }
   );
}
