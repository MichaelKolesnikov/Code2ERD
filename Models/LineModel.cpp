#include "LineModel.h"

LineModel::LineModel(int id, const QVector<QPointF> &nodes)
   : ERDItemModel(id)
   , QVector<QPointF>(nodes)
{

}
