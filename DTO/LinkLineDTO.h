#pragma once
#include "LineDTO.h"

struct LinkLineDTO : public LineDTO
{
   QString minCardinality;
   QString maxCardinality;

   bool operator==(const LinkLineDTO& other) const
   {
      return id == other.id && nodes == other.nodes && minCardinality == other.minCardinality && maxCardinality == other.maxCardinality;
   }
};
