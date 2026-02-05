#pragma once
#include "EntityItem.h"
class PropertyModel;

class PropertyItem : public EntityItem
{
   Q_OBJECT
public:
   enum {
      Type = UserType + PropertyItemType
   };
   int type() const override;

   explicit PropertyItem(PropertyModel*);

   virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
   PropertyModel* m_model;
};

