#include "EntityModel.h"

EntityModel::EntityModel(QString id, const QString &name, const QPointF &position, const QSize& size)
   : ERDItemModel(id)
   , NameHolder(name)
   , PositionHolder(position)
   , m_size(size)
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

QSize EntityModel::size() const
{
   return m_size;
}

void EntityModel::setSize(const QSize &size)
{
   m_size = size;
   emit sizeChanged();
}
