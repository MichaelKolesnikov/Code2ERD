#pragma once
#include "EntityItem.h"
class PropertyItemModel;

class PropertyItem : public EntityItem
{
   Q_OBJECT
public:
   enum {
      Type = UserType + PropertyItemType
   };
   int type() const override;

   explicit PropertyItem(PropertyItemModel*);

   virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
   PropertyItemModel* m_model;
};

