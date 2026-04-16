#pragma once
#include "ERDItem.h"
#include "IRectHolder.h"

class SelectionGroupItem : public ERDItem, public IRectHolder
{
   Q_OBJECT
   Q_INTERFACES(IRectHolder)

public:
   enum { Type = UserType + ERDItemType::SelectionGroupItemType };

   int type() const override;

   explicit SelectionGroupItem();

   virtual QRectF boundingRect() const override;

   virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

   void setRect(QRectF) override;

private:
   QRectF m_rect;
};
