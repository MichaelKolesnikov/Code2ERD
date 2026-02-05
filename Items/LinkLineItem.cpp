#include "LinkLineItem.h"
#include "Models/LinkLineModel.h"
#include <QPainter>

LinkLineItem::LinkLineItem(LinkLineModel *model) : LineItem(model), m_model(model)
{

}

void LinkLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
   painter->save();
   painter->drawText(m_model->at(0), m_model->minCardinality() + "," + m_model->maxCardinality());
   painter->setPen(QPen(Qt::black, 1.5));
   painter->drawPath(painterPath());
   painter->restore();
}
