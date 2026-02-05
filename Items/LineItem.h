#pragma once
#include "ERDItem.h"
class LineModel;

class LineItem : public ERDItem
{
   Q_OBJECT
public:
   explicit LineItem(LineModel*);

   virtual QRectF boundingRect() const override;
   virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
   QPainterPath shape() const override;

   QPainterPath painterPath() const;

private:
   LineModel* m_model;
};

