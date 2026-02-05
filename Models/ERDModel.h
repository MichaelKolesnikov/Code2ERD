#pragma once
#include "LinkModel.h"
#include "PropertyModel.h"
#include "PropertyLineModel.h"
#include "LinkLineModel.h"
#include <QSet>

class ERDModel
{
public:
   explicit ERDModel();

   void add(EntityModel*);
   void add(PropertyModel*);
   void add(LinkModel*);
   void add(PropertyLineModel*);
   void add(LinkLineModel*);
   void remove(EntityModel*);
   void remove(PropertyModel*);
   void remove(LinkModel*);
   void remove(PropertyLineModel*);
   void remove(LinkLineModel*);

   const QSet<EntityModel*>& entities() const;
   const QSet<PropertyModel*>& properties() const;
   const QSet<LinkModel*>& links() const;
   const QSet<PropertyLineModel*>& propertyLines() const;
   const QSet<LinkLineModel*>& linkLines() const;

private:
   QSet<EntityModel*> m_entities;
   QSet<PropertyModel*> m_properties;
   QSet<LinkModel*> m_links;
   QSet<PropertyLineModel*> m_propertyLines;
   QSet<LinkLineModel*> m_linkLines;
};

