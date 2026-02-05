#include "EntityItemModel.h"

EntityItemModel::EntityItemModel(int id, const QString &name, const QPointF &position)
   : ERDItemModel(id)
   , NameHolder(name)
   , PositionHolder(position)
{

}
