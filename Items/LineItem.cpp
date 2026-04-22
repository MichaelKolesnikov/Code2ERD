#include "LineItem.h"
#include "Models/LineModel.h"
#include <QPainterPathStroker>
#include <QPen>
#include <QPainter>

LineItem::LineItem(LineModel *model) : ERDItem(model), m_model(model)
{
   connect(m_model, &LineModel::propertyChanged, [this](const char*)
   {
      prepareGeometryChange();
      update();
   }
   );
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
   auto curNodes = nodes();
   QPainterPath painterPath(curNodes[0]);

   for (int i = 1; i < curNodes.size(); ++i)
   {
      painterPath.lineTo(curNodes[i]);
   }
   return painterPath;
}

QVector<QPointF> LineItem::nodes() const
{
   return m_model->nodes();
}
