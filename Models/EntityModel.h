#pragma once
#include "ERDItemModel.h"
#include "Holders/NameHolder.h"
#include "Holders/PositionHolder.h"

class EntityModel : public ERDItemModel, public NameHolder, public PositionHolder
{
   Q_OBJECT
   Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
   Q_PROPERTY(QPointF position READ position WRITE setPosition NOTIFY positionChanged)

public:
   explicit EntityModel(QString id, const QString& name = "", const QPointF& position = QPointF(0, 0));

   void setName(const QString& name);
   void setPosition(const QPointF& pos);

signals:
   void nameChanged();
   void positionChanged();
};
