#include "LineItem.h"
#include "Models/LineItemModel.h"
#include <QPainterPathStroker>
#include <QPen>
#include <QPainter>

LineItem::LineItem(LineItemModel *model) : ERDItem(model), m_model(model)
{
}

QRectF LineItem::boundingRect() const
{
   return shape().boundingRect();
}

void LineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
   painter->save();
   painter->setPen(QPen(Qt::black, 1.5));
   painter->drawPath(painterPath());
   painter->restore();
}

QPainterPath LineItem::shape() const
{
   static QPainterPathStroker painterPathStroker(QPen(Qt::black, 10));
   return painterPathStroker.createStroke(painterPath());
}

QPainterPath LineItem::painterPath() const
{
   QPainterPath painterPath;
   for (auto node : *m_model)
   {
      painterPath.lineTo(node);
   }
   return painterPath;
}
