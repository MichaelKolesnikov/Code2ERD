#include "LinkLineModel.h"

LinkLineModel::LinkLineModel(int id, const QPointF& position, const QVector<int> &moves, int entityId, int linkId, const QString &minCardinality, const QString &maxCardinality)
   : LineModel(id, position, moves)
   , m_entityId(entityId)
   , m_linkId(linkId)
   , m_minCardinality(minCardinality)
   , m_maxCardinality(maxCardinality)
{

}

int LinkLineModel::entityId() const
{
   return m_entityId;
}

void LinkLineModel::setEntityId(int newEntityId)
{
   m_entityId = newEntityId;
}

int LinkLineModel::linkId() const
{
   return m_linkId;
}

void LinkLineModel::setLinkId(int newLinkId)
{
   m_linkId = newLinkId;
}

const QString &LinkLineModel::minCardinality() const
{
   return m_minCardinality;
}

void LinkLineModel::setMinCardinality(const QString &newMinCardinality)
{
   m_minCardinality = newMinCardinality;
}

const QString &LinkLineModel::maxCardinality() const
{
   return m_maxCardinality;
}

void LinkLineModel::setMaxCardinality(const QString &newMaxCardinality)
{
   m_maxCardinality = newMaxCardinality;
}
