#include "EntityModel.h"

EntityModel::EntityModel(int id, const QString &name, const QPointF &position)
   : ERDItemModel(id)
   , NameHolder(name)
   , PositionHolder(position)
{

}

void EntityModel::setName(const QString& name)
{
   if (NameHolder::name() == name) return;
   NameHolder::setName(name);
   emit nameChanged();
}

void EntityModel::setPosition(const QPointF& pos)
{
   if (PositionHolder::position() == pos) return;
   PositionHolder::setPosition(pos);
   emit positionChanged();
}
