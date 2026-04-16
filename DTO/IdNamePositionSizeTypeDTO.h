#pragma once
#include "ERDItemDTO.h"
#include <QPointF>
#include <QSizeF>

struct IdNamePositionSizeTypeDTO : public ERDItemDTO
{
   enum Type
   {
      Entity,
      Link,
      Property
   };

   QString name;

   QPointF position;

   QSizeF size;

   Type type;

   bool operator==(const IdNamePositionSizeTypeDTO& other) const
   {
      return

         id == other.id &&

         name == other.name &&

         position == other.position &&

         size == other.size &&

         type == other.type;
   }
};
