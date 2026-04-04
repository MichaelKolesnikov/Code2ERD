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

   const QSet<EntityModel*> &entities() const;
   const QSet<PropertyModel*> &properties() const;
   const QSet<LinkModel*> &links() const;
   const QSet<LineModel*> &propertyLines() const;
   const QSet<LinkLineModel*> &linkLines() const;

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
   QSet<EntityModel*> m_entities;
   QSet<PropertyModel*> m_properties;
   QSet<LinkModel*> m_links;
   QSet<LineModel*> m_propertyLines;
   QSet<LinkLineModel*> m_linkLines;
   LinesConnectionsModel* m_linesConnectionsModel;
};

