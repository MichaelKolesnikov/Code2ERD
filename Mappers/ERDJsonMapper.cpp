#include "ERDJsonMapper.h"
#include "EntityMapper.h"
#include "LinkMapper.h"
#include "PropertyMapper.h"
#include "PropertyLineMapper.h"
#include "LinkLineMapper.h"
#include <QJsonArray>

std::optional<ERDModel> ERDMapper::fromJson(const QJsonObject &jsonObject)
{
   bool isValid =
      jsonObject.contains(entities) && jsonObject[entities].isArray() &&
      jsonObject.contains(links) && jsonObject[links].isArray() &&
      jsonObject.contains(properties) && jsonObject[properties].isArray() &&
      jsonObject.contains(propertyLines) && jsonObject[propertyLines].isArray() &&
      jsonObject.contains(linkLines) && jsonObject[linkLines].isArray();
   if (!isValid)
   {
      return std::nullopt;
   }
   ERDModel erdModel;
   for (auto json : jsonObject[entities].toArray())
   {
      if (!json.isObject())
      {
         return std::nullopt;
      }
      if (auto model = EntityMapper::fromJson(json.toObject()))
      {
         erdModel.add(new EntityModel(model.value()));
      }
      else
      {
         return std::nullopt;
      }
   }
   for (auto json : jsonObject[properties].toArray())
   {
      if (!json.isObject())
      {
         return std::nullopt;
      }
      if (auto model = PropertyMapper::fromJson(json.toObject()))
      {
         erdModel.add(new PropertyModel(model.value()));
      }
      else
      {
         return std::nullopt;
      }
   }
   for (auto json : jsonObject[links].toArray())
   {
      if (!json.isObject())
      {
         return std::nullopt;
      }
      if (auto model = LinkMapper::fromJson(json.toObject()))
      {
         erdModel.add(new LinkModel(model.value()));
      }
      else
      {
         return std::nullopt;
      }
   }
   for (auto json : jsonObject[propertyLines].toArray())
   {
      if (!json.isObject())
      {
         return std::nullopt;
      }
      if (auto model = PropertyLineMapper::fromJson(json.toObject()))
      {
         erdModel.add(new PropertyLineModel(model.value()));
      }
      else
      {
         return std::nullopt;
      }
   }
   for (auto json : jsonObject[linkLines].toArray())
   {
      if (!json.isObject())
      {
         return std::nullopt;
      }
      if (auto model = LinkLineMapper::fromJson(json.toObject()))
      {
         erdModel.add(new LinkLineModel(model.value()));
      }
      else
      {
         return std::nullopt;
      }
   }
   return erdModel;
}

template <class Mapper, class T>
QJsonArray f(const QSet<T>& v)
{
   QJsonArray array;
   for (auto model : v)
   {
      array.append(Mapper::toJson(*model));
   }
   return array;
}

QJsonObject ERDMapper::toJson(const ERDModel &erdModel)
{
   QJsonObject answer;

   answer[entities] = f<EntityMapper>(erdModel.entities());
   answer[properties] = f<PropertyMapper>(erdModel.properties());
   answer[links] = f<LinkMapper>(erdModel.links());
   answer[propertyLines] = f<PropertyLineMapper>(erdModel.propertyLines());
   answer[linkLines] = f<LinkLineMapper>(erdModel.linkLines());

   return answer;
}
