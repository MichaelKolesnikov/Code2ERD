#pragma once
#include "ERDItemModel.h"
#include <QVector>
#include <QPointF>

class LineModel : public ERDItemModel, public QVector<QPointF>
{
public:
   explicit LineModel(int id, const QVector<QPointF>& nodes);
};

