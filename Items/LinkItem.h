#pragma once
#include "EntityItem.h"
class LinkModel;

class LinkItem : public EntityItem
{
   Q_OBJECT
public:
   enum {
      Type = UserType + LinkItemType
   };
   int type() const override;

   explicit LinkItem(LinkModel*);

   virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
   LinkModel* m_model;
};

