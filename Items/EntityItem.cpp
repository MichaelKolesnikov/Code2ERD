#include "EntityItem.h"
#include <QPainter>

int EntityItem::type() const
{
   return EntityItem::Type;
}

EntityItem::EntityItem(EntityModel* model) : ERDItem(model), m_model(model)
{
   setPos(model->position());
   connect(this, &EntityItem::xChanged, this, [this](){  });
   connect(this, &EntityItem::yChanged, this, [this](){  });
   connect(this, &EntityItem::zChanged, this, [this](){  });
}

QRectF EntityItem::boundingRect() const
{
   QRectF rect(-m_model->size().width() / 2, -m_model->size().height() / 2, m_model->size().width(), m_model->size().height());
   return rect;
}

void EntityItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
   painter->save();
   painter->setBrush(Qt::gray);
   painter->drawRect(boundingRect());
   painter->setBrush(Qt::black);
   painter->drawText(boundingRect(), Qt::AlignCenter, m_model->name());
   painter->restore();
}
