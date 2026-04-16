#pragma once
#include "ERDItem.h"
#include "IRectHolder.h"
#include "Models/IdNamePositionSizeTypeModel.h"

class EntityItem : public ERDItem, public IRectHolder
{
   Q_OBJECT
   Q_INTERFACES(IRectHolder)
public:
   enum {
      Type = UserType + EntityItemType
   };
   int type() const override;

   explicit EntityItem(IdNamePositionSizeTypeModel*);

   virtual QRectF boundingRect() const override;
   virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

   virtual void setRect(QRectF) override;

private:
   void paintAsEntity(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);

   void paintAsLink(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);

   void paintAsProperty(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);

   IdNamePositionSizeTypeModel* m_model;
};

