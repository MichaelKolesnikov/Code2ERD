#pragma once
#include "Core_global.h"
#include <QMap>
#include <QPointF>
#include <QSizeF>
#include <QString>
struct DiagramDTO;

namespace AutoLayout
{
   // Places the entities and the relationships so that connected ones are near each other, then puts the
   // attributes around their owner in the free directions between the lines, the parts of a composite attribute
   // further out. The same diagram gives the same picture.
   // sizes: node name (see LayoutDTO) -> size of the shape. Returns node name -> center of the shape
   CORE_EXPORT QMap<QString, QPointF> arrange(const DiagramDTO&, const QMap<QString, QSizeF>& sizes);

   // Keeps the known positions and places only the nodes without one, next to what they are connected to
   CORE_EXPORT QMap<QString, QPointF> complete(const DiagramDTO&, const QMap<QString, QSizeF>& sizes, const QMap<QString, QPointF>& known);
}
