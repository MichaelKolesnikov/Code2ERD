#pragma once
#include "LineModel.h"

class LinkLineModel : public LineModel
{
   Q_OBJECT
   Q_PROPERTY(QString minCardinality READ minCardinality WRITE setMinCardinality NOTIFY minCardinalityChanged)
   Q_PROPERTY(QString maxCardinality READ maxCardinality WRITE setMaxCardinality NOTIFY maxCardinalityChanged)

public:
   explicit LinkLineModel(int id, const QVector<QPointF>& points, const QString &minCardinality, const QString &maxCardinality);

   const QString &minCardinality() const;
   void setMinCardinality(const QString &newMinCardinality);
   const QString &maxCardinality() const;
   void setMaxCardinality(const QString &newMaxCardinality);

signals:
   void minCardinalityChanged();
   void maxCardinalityChanged();

private:
   QString m_minCardinality;
   QString m_maxCardinality;
};
