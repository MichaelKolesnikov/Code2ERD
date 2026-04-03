#include "PropertyLineModel.h"

PropertyLineModel::PropertyLineModel(int id, const QVector<QPointF> &moves, int entityId, int propertyId)
   : LineModel(id, moves)
   , m_entityId(entityId)
   , m_propertyId(propertyId)
{

}

int PropertyLineModel::entityId() const
{
   return m_entityId;
}

void PropertyLineModel::setEntityId(int newEntityId)
{
   m_entityId = newEntityId;
}

int PropertyLineModel::propertyId() const
{
   return m_propertyId;
}

void PropertyLineModel::setPropertyId(int newPropertyId)
{
   m_propertyId = newPropertyId;
}
