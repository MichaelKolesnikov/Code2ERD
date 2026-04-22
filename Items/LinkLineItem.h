#pragma once
#include "LineItem.h"
class LinkLineModel;

class LinkLineItem : public LineItem
{
   Q_OBJECT
public:
   enum { Type = UserType + ERDItemType::LinkLineItemType };

   int type() const override;

   explicit LinkLineItem(LinkLineModel*);

   virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
   LinkLineModel* m_model;
};

