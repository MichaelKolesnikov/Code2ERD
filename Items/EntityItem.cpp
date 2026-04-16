#include "EntityItem.h"
#include <QPainter>

int EntityItem::type() const
{
   return EntityItem::Type;
}

EntityItem::EntityItem(IdNamePositionSizeTypeModel* model) : ERDItem(model), m_model(model)
{
   setPos(model->position());
   connect(this, &EntityItem::xChanged, this, [this](){ m_model->setPosition(pos()); });
   connect(this, &EntityItem::yChanged, this, [this](){ m_model->setPosition(pos()); });

   connect(model, &IdNamePositionSizeTypeModel::nameChanged, [this]() {update();});
   connect(model, &IdNamePositionSizeTypeModel::positionChanged, [this]() {update();});
   connect(model, &IdNamePositionSizeTypeModel::sizeChanged, [this]() {prepareGeometryChange(); update();});
}

QRectF EntityItem::boundingRect() const
{
   QRectF rect(-m_model->size().width() / 2, -m_model->size().height() / 2, m_model->size().width(), m_model->size().height());
   return rect;
}

void EntityItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
   using PaintMethod = void(EntityItem::*)(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
   static const PaintMethod methods[] = {&EntityItem::paintAsEntity, &EntityItem::paintAsLink, &EntityItem::paintAsProperty};
   (this->*methods[static_cast<int>(m_model->type())])(painter, option, widget);
}

void EntityItem::setRect(QRectF rect)
{
   if (boundingRect() == rect)
   {
      return;
   }
   prepareGeometryChange();
   m_model->setSize(QSizeF(rect.width(), rect.height()));
   update();
}

void EntityItem::paintAsEntity(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
   painter->save();
   painter->setBrush(Qt::gray);
   painter->drawRect(boundingRect());
   painter->setBrush(Qt::black);
   painter->drawText(boundingRect(), Qt::AlignCenter, m_model->name());
   painter->restore();
}

void EntityItem::paintAsLink(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
   painter->save();
   auto r = boundingRect();
   static auto diamond = QPolygonF({
      QPointF(r.left(), r.center().y()),
      QPointF(r.center().x(), r.top()),
      QPointF(r.right(), r.center().y()),
      QPointF(r.center().x(), r.bottom())
   });
   painter->setBrush(Qt::gray);
   painter->drawPolygon(diamond);
   painter->setBrush(Qt::black);
   painter->drawText(boundingRect(), Qt::AlignCenter, m_model->name());
   painter->restore();
}

void EntityItem::paintAsProperty(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
   painter->save();
   painter->setBrush(Qt::gray);
   painter->drawEllipse(boundingRect());
   painter->setBrush(Qt::black);
   painter->drawText(boundingRect(), Qt::AlignCenter, m_model->name());
   painter->restore();
}
