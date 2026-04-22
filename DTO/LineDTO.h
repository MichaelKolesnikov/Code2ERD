#pragma once
#include "ERDItemDTO.h"
#include <QPointF>
#include <QVector>

struct LineDTO : public ERDItemDTO
{
   QVector<QPointF> nodes;

   bool operator==(const LineDTO& other) const
   {
      return id == other.id && nodes == other.nodes;
   }
};
