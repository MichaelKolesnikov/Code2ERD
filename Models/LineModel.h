#pragma once
#include "ERDItemModel.h"
#include "DTO/LineDTO.h"

class LineModel : public ERDItemModel
{
   Q_OBJECT

   Q_PROPERTY(QString id READ id CONSTANT)

   Q_PROPERTY(QVector<QPointF> nodes READ nodes WRITE setNodes NOTIFY nodesChanged)


public:
   explicit LineModel(const LineDTO& dto = LineDTO());
   ~LineModel() = default;

   const LineDTO& dto() const { return m_dto; }

   // get

   const QString& id() const { return m_dto.id; }

   const QVector<QPointF>& nodes() const { return m_dto.nodes; }


   // set

   void setNodes(const QVector<QPointF>& value);


signals:

   void nodesChanged();

private:
   LineDTO m_dto;
};
