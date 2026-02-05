#include "PropertyLineModel.h"

PropertyLineModel::PropertyLineModel(int id, const QVector<QPointF> &nodes, int entityId, int propertyId)
   : LineModel(id, nodes)
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
