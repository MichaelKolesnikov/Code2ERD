#include "PropertyItem.h"
#include "Models/PropertyItemModel.h"
#include <QPainter>

int PropertyItem::type() const
{
   return PropertyItem::Type;
}

PropertyItem::PropertyItem(PropertyItemModel *model) : EntityItem(model), m_model(model)
{

}

void PropertyItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
   painter->save();
   painter->setBrush(Qt::gray);
   painter->drawEllipse(boundingRect());
   painter->setBrush(Qt::black);
   painter->drawText(boundingRect(), Qt::AlignCenter, m_model->name());
   painter->restore();
}
