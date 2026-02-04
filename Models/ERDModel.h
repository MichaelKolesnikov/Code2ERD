#pragma once
#include "LinkItemModel.h"
#include "PropertyItemModel.h"
#include "PropertyLineItemModel.h"
#include "LinkLineItemModel.h"
#include <QSet>

class ERDModel
{
public:
   explicit ERDModel();

   void add(EntityItemModel*);
   void add(PropertyItemModel*);
   void add(LinkItemModel*);
   void add(PropertyLineItemModel*);
   void add(LinkLineItemModel*);
   void remove(EntityItemModel*);
   void remove(PropertyItemModel*);
   void remove(LinkItemModel*);
   void remove(PropertyLineItemModel*);
   void remove(LinkLineItemModel*);

   const QSet<EntityItemModel*>& entities() const;
   const QSet<PropertyItemModel*>& properties() const;
   const QSet<LinkItemModel*>& links() const;
   const QSet<PropertyLineItemModel*>& propertyLines() const;
   const QSet<LinkLineItemModel*>& linkLines() const;

private:
   QSet<EntityItemModel*> m_entities;
   QSet<PropertyItemModel*> m_properties;
   QSet<LinkItemModel*> m_links;
   QSet<PropertyLineItemModel*> m_propertyLines;
   QSet<LinkLineItemModel*> m_linkLines;
};

