#include "Models/LinkLineModel.h"

LinkLineModel::LinkLineModel(const LinkLineDTO& dto)
   : m_dto(dto)
{
}

void LinkLineModel::setMinCardinality(const QString& value)
{
   if (m_dto.minCardinality == value) return;
   m_dto.minCardinality = value;
   emit propertyChanged("minCardinality");
}

void LinkLineModel::setMaxCardinality(const QString& value)
{
   if (m_dto.maxCardinality == value) return;
   m_dto.maxCardinality = value;
   emit propertyChanged("maxCardinality");
}
