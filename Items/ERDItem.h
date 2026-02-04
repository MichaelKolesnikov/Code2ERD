#pragma once
#include <QGraphicsObject>
#include "Interfaces/IERDItem.h"
#include "Models/ERDItemModel.h"
#include "ERDItemType.h"
#include "ZValue.h"

class ERDItem : public QGraphicsObject, public IERDItem
{
   Q_OBJECT
   Q_INTERFACES(IERDItem)

public:
   explicit ERDItem(ERDItemModel*);

   virtual int id() const override final;

private:
   ERDItemModel* m_model;
};
