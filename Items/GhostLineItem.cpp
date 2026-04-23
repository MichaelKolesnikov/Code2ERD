#include "GhostLineItem.h"
#include "LineImpl.h"
#include <QPainter>

int GhostLineItem::type() const
{
   return Type;
}

GhostLineItem::GhostLineItem() : AncillaryItem(nullptr), m_nodes({})
{
}

GhostLineItem::GhostLineItem(const QVector<QPointF> &nodes) : AncillaryItem(nullptr), m_nodes(nodes)
{
}

QRectF GhostLineItem::boundingRect() const
{
   return LineImpl::boundingRect(nodes());
}

void GhostLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
   painter->save();
   LineImpl::paint(nodes(), painter, option, widget);

   QPen pen;
   pen.setColor(QColor(100, 160, 255, 220));
   pen.setWidth(4);
   pen.setStyle(Qt::SolidLine);
   pen.setCapStyle(Qt::RoundCap);
   pen.setJoinStyle(Qt::RoundJoin);
   painter->setPen(pen);
   painter->setBrush(Qt::NoBrush);

   const QVector<QPointF>& ps = nodes();
   for (int i = 1; i < ps.size(); ++i)
   {
      painter->drawLine(ps[i - 1], ps[i]);
   }

   QPen hatchPen;
   hatchPen.setColor(QColor(140, 190, 255, 160));
   hatchPen.setWidth(1);
   hatchPen.setStyle(Qt::DashLine);
   painter->setPen(hatchPen);

   for (int i = 1; i < ps.size(); ++i)
   {
      QLineF line(ps[i - 1], ps[i]);

      QLineF lineAbove(line.p1() - QPointF(0, 2), line.p2() - QPointF(0, 2));
      painter->drawLine(lineAbove);

      QLineF lineBelow(line.p1() + QPointF(0, 2), line.p2() + QPointF(0, 2));
      painter->drawLine(lineBelow);
   }
   painter->restore();
}

QPainterPath GhostLineItem::shape() const
{
   return LineImpl::shape(nodes());
}

const QVector<QPointF> &GhostLineItem::nodes() const
{
   return m_nodes;
}

void GhostLineItem::setNodes(const QVector<QPointF> &nodes)
{
   if (m_nodes == nodes)
   {
      return;
   }
   prepareGeometryChange();
   m_nodes = nodes;
   update();
}
