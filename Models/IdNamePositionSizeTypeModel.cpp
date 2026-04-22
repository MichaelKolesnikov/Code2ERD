#include "Models/IdNamePositionSizeTypeModel.h"

IdNamePositionSizeTypeModel::IdNamePositionSizeTypeModel(const IdNamePositionSizeTypeDTO& dto)
   : ERDItemModel(dto), m_dto(dto)
{
}


void IdNamePositionSizeTypeModel::setName(const QString& value)
{
   if (m_dto.name == value) return;
   m_dto.name = value;
   emit propertyChanged("name");
}

void IdNamePositionSizeTypeModel::setPosition(const QPointF& value)
{
   if (m_dto.position == value) return;
   m_dto.position = value;
   emit propertyChanged("position");
}

void IdNamePositionSizeTypeModel::setSize(const QSizeF& value)
{
   if (m_dto.size == value) return;
   m_dto.size = value;
   emit propertyChanged("size");
}
