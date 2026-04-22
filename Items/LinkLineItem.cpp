#include "LinkLineItem.h"
#include "Models/LinkLineModel.h"
#include <QPainter>

int LinkLineItem::type() const
{
   return Type;
}

LinkLineItem::LinkLineItem(LinkLineModel *model) : LineItem(model), m_model(model)
{

}

void LinkLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
   painter->save();
   if (!nodes().isEmpty())
   {
      painter->drawText(m_model->nodes()[0], m_model->minCardinality() + "," + m_model->maxCardinality());
   }
   painter->setPen(QPen(Qt::black, 1.5));
   painter->drawPath(painterPath());
   painter->restore();
}
