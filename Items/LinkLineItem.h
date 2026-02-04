#pragma once
#include "LineItem.h"
class LinkLineItemModel;

class LinkLineItem : public LineItem
{
   Q_OBJECT
public:
   explicit LinkLineItem(LinkLineItemModel*);

   virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
   LinkLineItemModel* m_model;
};

