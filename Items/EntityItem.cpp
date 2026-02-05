#include "EntityItem.h"
#include <QPainter>

int EntityItem::type() const
{
   return EntityItem::Type;
}

EntityItem::EntityItem(EntityModel* model) : ERDItem(model), m_model(model)
{
   setPos(model->position());
   connect(this, &EntityItem::xChanged, this, [this](){ m_model->setPosition(scenePos()); });
   connect(this, &EntityItem::yChanged, this, [this](){ m_model->setPosition(scenePos()); });
   connect(this, &EntityItem::zChanged, this, [this](){ m_model->setPosition(scenePos()); });
}

QRectF EntityItem::boundingRect() const
{
   static auto rect = QRectF(-50, -30, 100, 60);
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
