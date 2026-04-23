#include "AnchorItem.h"
#include "ZValue.h"
#include <QCursor>
#include <QPainter>

int AnchorItem::type() const
{
   return AnchorItem::Type;
}

AnchorItem::Constraint AnchorItem::constraintType() const
{
   return m_constraint;
}

AnchorItem::AnchorItem(Constraint constraint)
   : AncillaryItem(nullptr), m_constraint(constraint)
{
   setFlag(QGraphicsItem::ItemIgnoresTransformations);
   setAcceptHoverEvents(true);
   switch (constraint)
   {
   case Horizontal:
      setCursor(Qt::SizeHorCursor);
      break;
   case Vertical:
      setCursor(Qt::SizeVerCursor);
      break;
   case Y_MINUS_X:
      setCursor(Qt::SizeFDiagCursor);
      break;
   case Y_X:
      setCursor(Qt::SizeBDiagCursor);
      break;
   case NoConstraint:
      setCursor(Qt::DragMoveCursor);
   }
   setZValue(ZValue::AnchorItemZValue);
}

QRectF AnchorItem::boundingRect() const
{
   static auto rect = QRectF(-4, -4, 8, 8);
   return rect;
}

void AnchorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
   painter->save();
   painter->fillRect(boundingRect(), Qt::green);
   painter->restore();
}
