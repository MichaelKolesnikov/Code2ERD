#pragma once
#include "LineItemModel.h"

class LinkLineItemModel : public LineItemModel
{
public:
   explicit LinkLineItemModel(int id, const QVector<QPointF>& nodes, int entityId, int linkId, const QString &minCardinality, const QString &maxCardinality);

   int entityId() const;
   void setEntityId(int newEntityId);
   int linkId() const;
   void setLinkId(int newLinkId);
   const QString &minCardinality() const;
   void setMinCardinality(const QString &newMinCardinality);
   const QString &maxCardinality() const;
   void setMaxCardinality(const QString &newMaxCardinality);

private:
   int m_entityId;
   int m_linkId;
   QString m_minCardinality;
   QString m_maxCardinality;
};
