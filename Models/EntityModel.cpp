#include "EntityModel.h"

EntityModel::EntityModel(int id, const QString &name, const QPointF &position)
   : ERDItemModel(id)
   , NameHolder(name)
   , PositionHolder(position)
{

}
