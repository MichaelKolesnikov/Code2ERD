#pragma once
#include "ERDItemModel.h"
#include "DTO/IdNamePositionSizeTypeDTO.h"

class IdNamePositionSizeTypeModel : public ERDItemModel
{
   Q_OBJECT

   Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

   Q_PROPERTY(QPointF position READ position WRITE setPosition NOTIFY positionChanged)

   Q_PROPERTY(QSizeF size READ size WRITE setSize NOTIFY sizeChanged)

   Q_PROPERTY(int type READ type CONSTANT)

public:
   explicit IdNamePositionSizeTypeModel(const IdNamePositionSizeTypeDTO& dto = IdNamePositionSizeTypeDTO());
   ~IdNamePositionSizeTypeModel() = default;

   const IdNamePositionSizeTypeDTO& dto() const { return m_dto; }

   // get

   const QString& id() const { return m_dto.id; }

   const QString& name() const { return m_dto.name; }

   const QPointF& position() const { return m_dto.position; }

   const QSizeF& size() const { return m_dto.size; }

   int type() const { return m_dto.type; }


   // set

   void setName(const QString& value);

   void setPosition(const QPointF& value);

   void setSize(const QSizeF& value);


signals:

   void nameChanged();

   void positionChanged();

   void sizeChanged();

private:
   IdNamePositionSizeTypeDTO m_dto;
};
