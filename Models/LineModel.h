#pragma once
#include "ERDItemModel.h"
#include <QVector>
#include <QPointF>

class LineItemModel : public ERDItemModel, public QVector<QPointF>
{
public:
   explicit LineItemModel(int id, const QVector<QPointF>& nodes);
};

