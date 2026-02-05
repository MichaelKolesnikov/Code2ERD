#pragma once
#include "ERDItem.h"
#include "Models/EntityModel.h"

class EntityItem : public ERDItem
{
   Q_OBJECT
public:
   enum {
      Type = UserType + EntityItemType
   };
   int type() const override;

   explicit EntityItem(EntityModel*);

   virtual QRectF boundingRect() const override;
   virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
   EntityModel* m_model;
};

