#pragma once
#include "ERDItemModel.h"
#include "Holders/PositionHolder.h"
#include <QVector>

class LineModel : public ERDItemModel, public PositionHolder, public QVector<int>
{
public:
   explicit LineModel(int id, const QPointF& position, const QVector<int>& moves);
};
