#pragma once
#include "ERDItemModel.h"
#include <QVector>
#include <QPointF>

Q_DECLARE_METATYPE(QVector<int>)

class LineModel : public ERDItemModel, public QVector<QPointF>
{
   Q_OBJECT
   Q_PROPERTY(QVector<QPointF> nodes READ nodes WRITE setNodes NOTIFY nodesChanged)

public:
   explicit LineModel(int id, const QVector<QPointF>& points);

   QVector<QPointF> nodes() const;
   void setNodes(const QVector<QPointF>& points);

signals:
   void nodesChanged();

private:
   QVector<QPointF> m_nodes;
};
