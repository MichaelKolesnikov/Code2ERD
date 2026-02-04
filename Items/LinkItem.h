#pragma once
#include "EntityItem.h"
class LinkItemModel;

class LinkItem : public EntityItem
{
   Q_OBJECT
public:
   enum {
      Type = UserType + LinkItemType
   };
   int type() const override;

   explicit LinkItem(LinkItemModel*);

   virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
   LinkItemModel* m_model;
};

