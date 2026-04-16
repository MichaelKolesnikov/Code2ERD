#pragma once
#include <QObject>
#include "DTO/ERDItemDTO.h"

class ERDItemModel : public QObject
{
   Q_OBJECT

   Q_PROPERTY(QString id READ id CONSTANT)


public:
   explicit ERDItemModel(const ERDItemDTO& dto = ERDItemDTO());
   ~ERDItemModel() = default;

   const ERDItemDTO& dto() const { return m_dto; }

   const QString& id() const { return m_dto.id; }

signals:

   void dtoChanged();

private:
   ERDItemDTO m_dto;
};
