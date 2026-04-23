#include "LineItem.h"
#include "Models/LineModel.h"
#include "LineImpl.h"

int LineItem::type() const
{
   return Type;
}

LineItem::LineItem(LineModel *model) : ERDItem(model), m_model(model)
{
   setFlag(QGraphicsItem::ItemIsSelectable);
   connect(m_model, &LineModel::propertyChanged, [this](const char*)
   {
      prepareGeometryChange();
      update();
   }
   );
}

QRectF LineItem::boundingRect() const
{
   return shape().boundingRect();
}

void LineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
   LineImpl::paint(nodes(), painter, option, widget);
}

QPainterPath LineItem::shape() const
{
   return LineImpl::shape(nodes());
}

const QVector<QPointF>& LineItem::nodes() const
{
   return m_model->nodes();
}
