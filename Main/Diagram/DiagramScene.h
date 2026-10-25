#pragma once
#include "Main_global.h"
#include <QGraphicsScene>
#include "PIMPL.h"
#include "DiagramStyle.h"
#include "DTO/DiagramDTO.h"
#include "Layout/LayoutDTO.h"
class QUndoStack;
class NodeItem;
class EdgeItem;

// The Chen diagram of the text. Rebuilt on every successful parse, the shapes keep their places
// by their names; a renamed entity, relationship or attribute keeps its place too
class MAIN_EXPORT DiagramScene : public QGraphicsScene
{
   Q_OBJECT
public:
   explicit DiagramScene(QObject* parent = nullptr);
   ~DiagramScene() override;

   void setDiagram(const DiagramDTO&);
   const DiagramDTO& diagram() const;
   bool hasNodes() const;

   // Centers of the shapes on the diagram
   LayoutDTO layout() const;
   // Replaces the positions; shapes without one are placed next to what they are connected to
   void setLayout(const LayoutDTO&);
   void autoArrange();

   const DiagramStyle& style() const;
   void setTheme(const Theme&);
   void setShowAttributes(bool);
   void setSnapToGrid(bool);
   void setDiagramFont(const QFont&);
   // Hides the grid, hover and selection while rendering to a file
   void setExporting(bool);

   // Moves on the diagram, separate from the text undo
   QUndoStack* undoStack() const;

   // Selects the element defined at the line of the text
   void selectByLine(int line);
   // Bounding rectangle of the visible shapes and lines
   QRectF contentRect() const;
   NodeItem* node(const QString& id) const;

   // ---- called by the items ----
   void nodeMoved(NodeItem*, const QPointF& delta);
   void moveFinished();
   void activate(int line);
   void setHoveredNode(NodeItem*);
   void setHoveredEdge(EdgeItem*);

signals:
   // The user moved something or arranged the diagram
   void layoutChanged();
   // An element was clicked: show its definition
   void lineActivated(int line);
   void autoArrangeRequested();
   void fitRequested();

protected:
   void drawBackground(QPainter*, const QRectF&) override;
   void contextMenuEvent(QGraphicsSceneContextMenuEvent*) override;

PIMPL
};
