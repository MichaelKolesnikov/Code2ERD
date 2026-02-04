#include "ERDItem.h"

ERDItem::ERDItem(ERDItemModel* model) : QGraphicsObject(nullptr), m_model(model)
{
   setZValue(ZValue::ERDItemZValue);
}

int ERDItem::id() const
{
   return m_model->id();
}
