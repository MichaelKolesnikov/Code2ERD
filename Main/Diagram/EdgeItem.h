#pragma once
#include <QGraphicsItem>
#include "ItemType.h"
struct DiagramStyle;
class DiagramScene;
class NodeItem;

// A line from a relationship to an entity, with the cardinality next to the entity,
// or from an owner to its attribute
class EdgeItem : public QGraphicsItem
{
public:
   enum { Type = ItemType::Edge };

   struct Info
   {
      QString cardinality;
      QString role;
      bool total = false; // double line
      int line = 0;
   };

   EdgeItem(NodeItem* from, NodeItem* to, const Info&, const DiagramStyle*);

   int type() const override;
   NodeItem* from() const;
   NodeItem* to() const;
   const Info& info() const;

   // Lines between the same two shapes curve away from each other: index of count
   void setParallel(int index, int count);
   void updatePath();
   void setHighlighted(bool);

   QRectF boundingRect() const override;
   QPainterPath shape() const override;
   void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;

protected:
   void hoverEnterEvent(QGraphicsSceneHoverEvent*) override;
   void hoverLeaveEvent(QGraphicsSceneHoverEvent*) override;
   void mousePressEvent(QGraphicsSceneMouseEvent*) override;
   void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;

private:
   DiagramScene* diagramScene() const;
   QRectF labelRect(const QString& text, const QPointF& center) const;

   NodeItem* m_from = nullptr;
   NodeItem* m_to = nullptr;
   Info m_info;
   const DiagramStyle* m_style = nullptr;
   int m_parallelIndex = 0;
   int m_parallelCount = 1;
   QPainterPath m_path;
   QPointF m_cardinalityAt;
   QPointF m_roleAt;
   bool m_highlighted = false;
};
