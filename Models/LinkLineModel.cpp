#include "LinkLineItemModel.h"

LinkLineItemModel::LinkLineItemModel(int id, const QVector<QPointF> &nodes, int entityId, int linkId, const QString &minCardinality, const QString &maxCardinality)
   : LineItemModel(id, nodes)
   , m_entityId(entityId)
   , m_linkId(linkId)
   , m_minCardinality(minCardinality)
   , m_maxCardinality(maxCardinality)
{

}

int LinkLineItemModel::entityId() const
{
   return m_entityId;
}

void LinkLineItemModel::setEntityId(int newEntityId)
{
   m_entityId = newEntityId;
}

int LinkLineItemModel::linkId() const
{
   return m_linkId;
}

void LinkLineItemModel::setLinkId(int newLinkId)
{
   m_linkId = newLinkId;
}

const QString &LinkLineItemModel::minCardinality() const
{
   return m_minCardinality;
}

void LinkLineItemModel::setMinCardinality(const QString &newMinCardinality)
{
   m_minCardinality = newMinCardinality;
}

const QString &LinkLineItemModel::maxCardinality() const
{
   return m_maxCardinality;
}

void LinkLineItemModel::setMaxCardinality(const QString &newMaxCardinality)
{
   m_maxCardinality = newMaxCardinality;
}
