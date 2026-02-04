#include "LinkItem.h"
#include "Models/LinkItemModel.h"
#include <QPainter>

int LinkItem::type() const
{
   return LinkItem::Type;
}

LinkItem::LinkItem(LinkItemModel *model) : EntityItem(model), m_model(model)
{

}

void LinkItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
   painter->save();
   static auto diamond = QPolygonF({
      QPointF(-50, 0),
      QPointF(0, 30),
      QPointF(50, 0),
      QPointF(0, -30)
   });
   painter->setBrush(Qt::gray);
   painter->drawPolygon(diamond);
   painter->setBrush(Qt::black);
   painter->drawText(boundingRect(), Qt::AlignCenter, m_model->name());
   painter->restore();
}
