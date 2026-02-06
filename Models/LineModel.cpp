#include "LineModel.h"

LineModel::LineModel(int id, const QPointF& position, const QVector<int>& moves)
   : ERDItemModel(id)
   , PositionHolder(position)
   , QVector<int>(moves)
{

}
