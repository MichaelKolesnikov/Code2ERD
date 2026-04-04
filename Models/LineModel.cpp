#include "LineModel.h"

LineModel::LineModel(int id, const QVector<QPointF>& moves)
   : ERDItemModel(id)
   , QVector<QPointF>(moves)
{

}

QVector<QPointF> LineModel::nodes() const
{
   return m_nodes;
}

void LineModel::setNodes(const QVector<QPointF>& points)
{
   if (m_nodes == points) return;
   m_nodes = points;
   emit nodesChanged();
}
