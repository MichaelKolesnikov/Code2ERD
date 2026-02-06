#include "LineItem.h"
#include "Models/LineModel.h"
#include <QPainterPathStroker>
#include <QPen>
#include <QPainter>

LineItem::LineItem(LineModel *model) : ERDItem(model), m_model(model)
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
   QPointF currentPoint = m_model->position();
   QVector<QPointF> answer = {currentPoint};
   int coordinateNumber = 0;
   for (const auto& move : *m_model)
   {
      currentPoint += QPointF(!coordinateNumber * move, coordinateNumber * move);
      answer.push_back(currentPoint);
      coordinateNumber = (coordinateNumber + 1) % 2;
   }
   return answer;
}
