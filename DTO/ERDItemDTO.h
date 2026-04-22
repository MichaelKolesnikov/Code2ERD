#pragma once
#include <QString>

struct ERDItemDTO
{
   QString id;

   bool operator==(const ERDItemDTO& other) const
   {
      return id == other.id;
   }
};
