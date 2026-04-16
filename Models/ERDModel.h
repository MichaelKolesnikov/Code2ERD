#pragma once
#include "IdNamePositionSizeTypeModel.h"
#include "LineModel.h"
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
   void add(IdNamePositionSizeTypeModel*);
   void add(LineModel*);
   void add(LinkLineModel*);
   void set(LinesConnectionsModel*);
   void remove(ERDItemModel*);
   void remove(IdNamePositionSizeTypeModel*);
   void remove(LineModel*);
   void remove(LinkLineModel*);

   void add(const IdNamePositionSizeTypeDTO&);
   void add(const LineDTO&);
   void add(const LinkLineDTO&);

   const QSet<IdNamePositionSizeTypeModel*> &entities() const;
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
   QSet<IdNamePositionSizeTypeModel*> m_entities;
   QSet<LineModel*> m_propertyLines;
   QSet<LinkLineModel*> m_linkLines;
   LinesConnectionsModel* m_linesConnectionsModel;
};

