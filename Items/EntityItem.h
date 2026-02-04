#pragma once
#include "ERDItem.h"
#include "Models/EntityItemModel.h"

class EntityItem : public ERDItem
{
   Q_OBJECT
public:
   enum {
      Type = UserType + EntityItemType
   };
   int type() const override;

   explicit EntityItem(EntityItemModel*);

   virtual QRectF boundingRect() const override;
   virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
   EntityItemModel* m_model;
};

