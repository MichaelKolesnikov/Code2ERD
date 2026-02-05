#include "LineItemModel.h"

LineItemModel::LineItemModel(int id, const QVector<QPointF> &nodes)
   : ERDItemModel(id)
   , QVector<QPointF>(nodes)
{

}
