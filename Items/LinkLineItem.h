#pragma once
#include "LineItem.h"
class LinkLineModel;

class LinkLineItem : public LineItem
{
   Q_OBJECT
public:
   explicit LinkLineItem(LinkLineModel*);

   virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
   LinkLineModel* m_model;
};

