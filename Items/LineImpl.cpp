#include "LineImpl.h"
#include <QPainter>
#include <QRectF>
#include <QPainterPath>

QRectF LineImpl::boundingRect(const QVector<QPointF>& nodes)
{
   return shape(nodes).boundingRect();
}

void LineImpl::paint(const QVector<QPointF>& nodes, QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
   painter->save();
   painter->setPen(QPen(Qt::black, 1.5));
   painter->drawPath(painterPath(nodes));
   painter->restore();
}

QPainterPath LineImpl::shape(const QVector<QPointF>& nodes)
{
   static QPainterPathStroker painterPathStroker(QPen(Qt::black, 10));
   return painterPathStroker.createStroke(painterPath(nodes));
}

QPainterPath LineImpl::painterPath(const QVector<QPointF>& nodes)
{
   if (nodes.size() == 0)
   {
      return QPainterPath();
   }
   QPainterPath painterPath(nodes[0]);
   for (int i = 1; i < nodes.size(); ++i)
   {
      painterPath.lineTo(nodes[i]);
   }
   return painterPath;
}
