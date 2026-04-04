#include "LinkLineModel.h"

LinkLineModel::LinkLineModel(QString id, const QVector<QPointF> &moves, const QString &minCardinality, const QString &maxCardinality)
   : LineModel(id, moves)
   , m_minCardinality(minCardinality)
   , m_maxCardinality(maxCardinality)
{

}

const QString &LinkLineModel::minCardinality() const
{
   return m_minCardinality;
}

void LinkLineModel::setMinCardinality(const QString &newMinCardinality)
{
   if (m_minCardinality == newMinCardinality) return;
   m_minCardinality = newMinCardinality;
   emit minCardinalityChanged();
}

const QString &LinkLineModel::maxCardinality() const
{
   return m_maxCardinality;
}

void LinkLineModel::setMaxCardinality(const QString &newMaxCardinality)
{
   if (m_maxCardinality == newMaxCardinality) return;
   m_maxCardinality = newMaxCardinality;
   emit maxCardinalityChanged();
}
