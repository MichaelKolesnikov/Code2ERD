#pragma once
#include "ERDItem.h"

class AnchorItem : public ERDItem
{
   Q_OBJECT
public:
   enum { Type = UserType + ERDItemType::AnchorItemType };

   int type() const override;

   enum Constraint
   {
      Y,
      X,
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
