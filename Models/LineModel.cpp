#include "Models/LineModel.h"

LineModel::LineModel(const LineDTO& dto)
   : ERDItemModel(dto), m_dto(dto)
{
}


void LineModel::setNodes(const QVector<QPointF>& value)
{
   if (m_dto.nodes == value) return;
   m_dto.nodes = value;
   emit nodesChanged();
   emit dtoChanged();
}
