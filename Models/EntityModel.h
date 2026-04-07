#pragma once
#include "ERDItemModel.h"
#include "Holders/NameHolder.h"
#include "Holders/PositionHolder.h"

#include <QSize>

class EntityModel : public ERDItemModel, public NameHolder, public PositionHolder
{
   Q_OBJECT
   Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
   Q_PROPERTY(QPointF position READ position WRITE setPosition NOTIFY positionChanged)

public:
   explicit EntityModel(QString id, const QString& name = "", const QPointF& position = QPointF(0, 0), const QSize& size = QSize(50, 40));

   void setName(const QString& name);
   virtual void setPosition(const QPointF& pos) override;

   QSize size() const;
   void setSize(const QSize&);

signals:
   void nameChanged();
   void positionChanged();
   void sizeChanged();

private:
   QSize m_size;
};
