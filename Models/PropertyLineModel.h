#pragma once
#include "LineModel.h"

class PropertyLineModel : public LineModel
{
public:
   explicit PropertyLineModel(int id, const QVector<QPointF>& nodes, int entityId, int propertyId);

   int entityId() const;
   void setEntityId(int newEntityId);
   int propertyId() const;
   void setPropertyId(int newPropertyId);

private:
   int m_entityId;
   int m_propertyId;
};

