#pragma once
#include "ERDItemModel.h"
#include "Holders/NameHolder.h"
#include "Holders/PositionHolder.h"

class EntityItemModel : public ERDItemModel, public NameHolder, public PositionHolder
{
public:
   explicit EntityItemModel(int id, const QString& name = "", const QPointF& position = QPointF(0, 0));
};
