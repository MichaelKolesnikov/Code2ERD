#include "LineItem.h"
#include "Models/LineModel.h"
#include <QPainterPathStroker>
#include <QPen>
#include <QPainter>

int LineItem::type() const
{
   return Type;
}

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
   if (nodes().size() == 0)
   {
      return QPainterPath();
   }
   QPainterPath painterPath(nodes()[0]);
   for (int i = 1; i < nodes().size(); ++i)
   {
      painterPath.lineTo(nodes()[i]);
   }
   return painterPath;
}

const QVector<QPointF>& LineItem::nodes() const
{
   return m_model->nodes();
}
