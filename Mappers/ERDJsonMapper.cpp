#include "ERDJsonMapper.h"
#include "EntityMapper.h"
#include "LinkMapper.h"
#include "PropertyMapper.h"
#include "LinkLineMapper.h"
#include "LineMapper.h"
#include "LinesConnectionsMapper.h"
#include <QJsonArray>

ERDModel* ERDMapper::fromJson(const QJsonObject &jsonObject)
{
   bool isValid =
      jsonObject.contains(entities) && jsonObject[entities].isArray() &&
      jsonObject.contains(links) && jsonObject[links].isArray() &&
      jsonObject.contains(properties) && jsonObject[properties].isArray() &&
      jsonObject.contains(propertyLines) && jsonObject[propertyLines].isArray() &&
      jsonObject.contains(linkLines) && jsonObject[linkLines].isArray() &&
      jsonObject.contains(linesConnections) && jsonObject[linesConnections].isArray();
   if (!isValid)
   {
      return nullptr;
   }
   ERDModel* erdModel;
   for (auto json : jsonObject[entities].toArray())
   {
      if (!json.isObject())
      {
         return nullptr;
      }
      if (auto model = EntityMapper::fromJson(json.toObject()))
      {
         erdModel->add(model);
      }
      else
      {
         return nullptr;
      }
   }
   for (auto json : jsonObject[properties].toArray())
   {
      if (!json.isObject())
      {
         return nullptr;
      }
      if (auto model = PropertyMapper::fromJson(json.toObject()))
      {
         erdModel->add(model);
      }
      else
      {
         return nullptr;
      }
   }
   for (auto json : jsonObject[links].toArray())
   {
      if (!json.isObject())
      {
         return nullptr;
      }
      if (auto model = LinkMapper::fromJson(json.toObject()))
      {
         erdModel->add(model);
      }
      else
      {
         return nullptr;
      }
   }
   for (auto json : jsonObject[propertyLines].toArray())
   {
      if (!json.isObject())
      {
         return nullptr;
      }
      if (auto model = LineMapper::fromJson(json.toObject()))
      {
         erdModel->add(model);
      }
      else
      {
         return nullptr;
      }
   }
   for (auto json : jsonObject[linkLines].toArray())
   {
      if (!json.isObject())
      {
         return nullptr;
      }
      if (auto model = LinkLineMapper::fromJson(json.toObject()))
      {
         erdModel->add(model);
      }
      else
      {
         return nullptr;
      }
   }
   for (auto json : jsonObject[linesConnections].toArray())
   {
      if (!json.isObject())
      {
         return nullptr;
      }
      if (auto model = LinesConnectionsMapper::fromJson(json.toObject()))
      {
         erdModel->set(model);
      }
      else
      {
         return nullptr;
      }
   }
   return erdModel;
}

template <class Mapper, class T>
QJsonArray f(const QList<T>& v)
{
   QJsonArray array;
   for (auto model : v)
   {
      array.append(Mapper::toJson(model));
   }
   return array;
}

QJsonObject ERDMapper::toJson(const ERDModel *erdModel)
{
   QJsonObject answer;

   answer[entities] = f<EntityMapper>(erdModel->entities());
   answer[properties] = f<PropertyMapper>(erdModel->properties());
   answer[links] = f<LinkMapper>(erdModel->links());
   answer[propertyLines] = f<LineMapper>(erdModel->propertyLines());
   answer[linkLines] = f<LinkLineMapper>(erdModel->linkLines());

   return answer;
}
