#pragma once
#include "AncillaryItem.h"

class GhostLineItem : public AncillaryItem
{
   Q_OBJECT

public:
   enum { Type = UserType + ERDItemType::GhostLineItemType };

   int type() const override;

   explicit GhostLineItem();
   explicit GhostLineItem(const QVector<QPointF>&);

   virtual QRectF boundingRect() const override;
   virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
   virtual QPainterPath shape() const override;

   const QVector<QPointF>& nodes() const;
   void setNodes(const QVector<QPointF>&);

private:
   QVector<QPointF> m_nodes;
};

