#pragma once
#include "ERDItemModel.h"
#include "DTO/LineDTO.h"

class LineModel : public ERDItemModel
{
   Q_OBJECT
   Q_PROPERTY(QString id READ id CONSTANT)
   Q_PROPERTY(QVector<QPointF> nodes READ nodes WRITE setNodes)

public:
   explicit LineModel(const LineDTO& dto = LineDTO());
   ~LineModel() = default;

   const LineDTO& dto() const { return m_dto; }

   const QString& id() const { return m_dto.id; }
   const QVector<QPointF>& nodes() const { return m_dto.nodes; }

   void setNodes(const QVector<QPointF>&);

private:
   LineDTO m_dto;
};
