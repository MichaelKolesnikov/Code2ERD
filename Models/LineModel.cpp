#include "LineModel.h"

LineModel::LineModel(int id, const QVector<QPointF>& moves)
   : ERDItemModel(id)
   , QVector<QPointF>(moves)
{

}
