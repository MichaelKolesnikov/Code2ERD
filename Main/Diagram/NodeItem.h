#pragma once
#include <QGraphicsItem>
#include <QColor>
#include "ItemType.h"
struct DiagramStyle;
class DiagramScene;

// What a shape shows: an entity, a relationship or an attribute
struct NodeInfo
{
   enum class Kind
   {
      Entity,
      Relationship,
      Attribute
   };

   Kind kind = Kind::Entity;
   QString id;   // the name in the layout: Student, Student.name.first
   QString name; // the text in the shape
   bool doubleBorder = false; // a weak entity, an identifying relationship, a multivalued attribute
   bool dashedBorder = false; // a derived attribute
   bool key = false;
   bool partialKey = false;
   QColor color;
   QString note;
   int line = 0;
   int endLine = 0;
};

// A rectangle, a diamond or an ellipse with a name; its position is its center
class NodeItem : public QGraphicsItem
{
public:
   enum { Type = ItemType::Node };

   NodeItem(const NodeInfo&, const DiagramStyle*, NodeItem* owner);

   int type() const override;
   const NodeInfo& info() const;
   // The entity or relationship of an attribute, the composite attribute of a part
   NodeItem* owner() const;

   // Recomputes the size after a change of the style
   void relayout();
   QSizeF size() const;
   // Where a line from the center towards the point leaves the shape, in scene coordinates
   QPointF boundaryPoint(const QPointF& towards) const;

   void setHighlighted(bool);

   QRectF boundingRect() const override;
   QPainterPath shape() const override;
   void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;

protected:
   QVariant itemChange(GraphicsItemChange, const QVariant&) override;
   void hoverEnterEvent(QGraphicsSceneHoverEvent*) override;
   void hoverLeaveEvent(QGraphicsSceneHoverEvent*) override;
   void mousePressEvent(QGraphicsSceneMouseEvent*) override;
   void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;

private:
   DiagramScene* diagramScene() const;
   QPainterPath outline(qreal inset) const;
   QColor fillColor() const;
   QColor borderColor() const;
   QString tooltip() const;

   NodeInfo m_info;
   const DiagramStyle* m_style = nullptr;
   NodeItem* m_owner = nullptr;
   QSizeF m_size;
   QPointF m_lastPos;
   bool m_highlighted = false;
   bool m_moved = false;
};
