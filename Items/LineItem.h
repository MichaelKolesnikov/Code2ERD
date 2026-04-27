#pragma once
#include "ERDItem.h"
class LineModel;

class LineItem : public ERDItem
{
   Q_OBJECT

public:
   enum { Type = UserType + ItemType::PropertyLineItemType };

   int type() const override;

   explicit LineItem(LineModel*);

   virtual QRectF boundingRect() const override;
   virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
   virtual QPainterPath shape() const override;

   const QVector<QPointF>& nodes() const;

private:
   LineModel* m_model;
};

