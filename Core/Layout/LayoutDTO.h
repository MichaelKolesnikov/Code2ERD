#pragma once
#include <QMap>
#include <QPointF>
#include <QString>

// Where the shapes are: the text does not keep positions, they are saved next to it.
// Entities and relationships go by their names, attributes by attributeKey(): Student.name.first
struct LayoutDTO
{
   QMap<QString, QPointF> nodes; // name -> center of the shape

   bool isEmpty() const
   {
      return nodes.isEmpty();
   }
};
