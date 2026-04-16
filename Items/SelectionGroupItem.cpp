#include "SelectionGroupItem.h"
#include "ZValue.h"
#include <QPainter>

int SelectionGroupItem::type() const
{
   return SelectionGroupItem::Type;
}

SelectionGroupItem::SelectionGroupItem()
   : ERDItem(nullptr)
{
   setFlag(QGraphicsItem::ItemSendsGeometryChanges);
   setFlag(QGraphicsItem::ItemIsFocusable, false);
   setZValue(ZValue::SelectionGroupItemZValue);
}

QRectF SelectionGroupItem::boundingRect() const
{
   return m_rect;
}

void SelectionGroupItem::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
   painter->save();
   static QPen pen(QColor(0, 255, 0), 1, Qt::DashLine);
   pen.setCosmetic(true);
   painter->setPen(pen);
   painter->drawRect(m_rect);
   painter->restore();
}

void SelectionGroupItem::setRect(QRectF newRect)
{
   if (m_rect != newRect)
   {
      prepareGeometryChange();
      m_rect = newRect;
      update();
   }
}
