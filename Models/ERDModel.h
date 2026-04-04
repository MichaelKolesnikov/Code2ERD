#pragma once
#include "LinkModel.h"
#include "PropertyModel.h"
#include "LinkLineModel.h"
#include "LinesConnectionsModel.h"
#include <QMap>
#include <QList>

class ERDModel : public QObject
{
   Q_OBJECT
public:
   explicit ERDModel();

   void add(ERDItemModel*);
   void add(EntityModel*);
   void add(PropertyModel*);
   void add(LinkModel*);
   void add(LineModel*);
   void add(LinkLineModel*);
   void set(LinesConnectionsModel*);
   void remove(ERDItemModel*);
   void remove(EntityModel*);
   void remove(PropertyModel*);
   void remove(LinkModel*);
   void remove(LineModel*);
   void remove(LinkLineModel*);

   QList<EntityModel*> entities() const;
   QList<PropertyModel*> properties() const;
   QList<LinkModel*> links() const;
   QList<LineModel*> propertyLines() const;
   QList<LinkLineModel*> linkLines() const;

   ERDItemModel* getItemById(int);

signals:
   void added(ERDItemModel*);
//   void added(EntityModel*);
//   void added(PropertyModel*);
//   void added(LinkModel*);
//   void added(LineModel*);
//   void added(LinkLineModel*);
   void seted(LinesConnectionsModel*);
   void removed(ERDItemModel*);
//   void removed(EntityModel*);
//   void removed(PropertyModel*);
//   void removed(LinkModel*);
//   void removed(LineModel*);
//   void removed(LinkLineModel*);
private:
   QMap<int, EntityModel*> m_entities;
   QMap<int, PropertyModel*> m_properties;
   QMap<int, LinkModel*> m_links;
   QMap<int, LineModel*> m_propertyLines;
   QMap<int, LinkLineModel*> m_linkLines;
   LinesConnectionsModel* m_linesConnectionsModel;
};

