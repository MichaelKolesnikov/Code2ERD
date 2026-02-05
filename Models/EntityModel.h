#pragma once
#include "ERDItemModel.h"
#include "Holders/NameHolder.h"
#include "Holders/PositionHolder.h"

class EntityModel : public ERDItemModel, public NameHolder, public PositionHolder
{
public:
   explicit EntityModel(int id, const QString& name = "", const QPointF& position = QPointF(0, 0));
};
