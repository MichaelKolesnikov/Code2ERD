#pragma once
#include "ERDItemModel.h"
#include "DTO/IdNamePositionSizeTypeDTO.h"

class IdNamePositionSizeTypeModel : public ERDItemModel
{
   Q_OBJECT
   Q_PROPERTY(QString name READ name WRITE setName)
   Q_PROPERTY(QPointF position READ position WRITE setPosition)
   Q_PROPERTY(QSizeF size READ size WRITE setSize)
   Q_PROPERTY(int type READ type CONSTANT)

public:
   explicit IdNamePositionSizeTypeModel(const IdNamePositionSizeTypeDTO& dto = IdNamePositionSizeTypeDTO());
   ~IdNamePositionSizeTypeModel() = default;

   const IdNamePositionSizeTypeDTO& dto() const { return m_dto; }


   const QString& id() const { return m_dto.id; }
   const QString& name() const { return m_dto.name; }
   const QPointF& position() const { return m_dto.position; }
   const QSizeF& size() const { return m_dto.size; }

   int type() const { return m_dto.type; }

   void setName(const QString& value);
   void setPosition(const QPointF& value);
   void setSize(const QSizeF& value);

private:
   IdNamePositionSizeTypeDTO m_dto;
};
