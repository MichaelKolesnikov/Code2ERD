#pragma once
#include "AncillaryItem.h"

class AnchorItem : public AncillaryItem
{
   Q_OBJECT
public:
   enum { Type = UserType + ItemType::AnchorItemType };

   int type() const override;

   enum Constraint
   {
      Horizontal,
      Vertical,
      Y_X,
      Y_MINUS_X,
      NoConstraint
   };

   Constraint constraintType() const;

   explicit AnchorItem(Constraint constraint);

   virtual QRectF boundingRect() const override;
   virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
   Constraint m_constraint;
};
