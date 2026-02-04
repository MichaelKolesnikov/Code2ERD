#include "PropertyLineItemModel.h"

PropertyLineItemModel::PropertyLineItemModel(int id, const QVector<QPointF> &nodes, int entityId, int propertyId)
   : LineItemModel(id, nodes)
   , m_entityId(entityId)
   , m_propertyId(propertyId)
{

}

int PropertyLineItemModel::entityId() const
{
   return m_entityId;
}

void PropertyLineItemModel::setEntityId(int newEntityId)
{
   m_entityId = newEntityId;
}

int PropertyLineItemModel::propertyId() const
{
   return m_propertyId;
}

void PropertyLineItemModel::setPropertyId(int newPropertyId)
{
   m_propertyId = newPropertyId;
}
