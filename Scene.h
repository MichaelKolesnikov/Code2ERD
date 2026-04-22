#pragma once
#include <QGraphicsScene>
#include <QMap>

class QUndoCommand;
class ERDItemModel;
class ERDModel;
class ERDItem;
class AnchorItem;
class Tool;

class Scene : public QGraphicsScene
{
   Q_OBJECT

public:
   using QGraphicsScene::QGraphicsScene;

   void loadModel(ERDModel* erdModel);

   ERDModel* erdModel();

   void pushCommand(QUndoCommand*);

   const Tool* currentTool() const;
   void setTool(Tool*);

signals:
   void signalToPushCommand(QUndoCommand*);

protected:
   virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
   virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
   virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
   virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

   void baseContextMenuEvent(QGraphicsSceneContextMenuEvent *event);
   void baseKeyPressEvent(QKeyEvent *event);
   void baseKeyReleaseEvent(QKeyEvent *event);
   void baseMousePressEvent(QGraphicsSceneMouseEvent *event);
   void baseMouseMoveEvent(QGraphicsSceneMouseEvent *event);
   void baseMouseReleaseEvent(QGraphicsSceneMouseEvent *event);
   void baseMouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
   void baseWheelEvent(QGraphicsSceneWheelEvent *event);

   friend class Tool;

private:
   Tool* m_tool = nullptr;

   struct Binding
   {
      ERDItemModel* erdItemModel;
      ERDItem* erdItem;
   };

   void addErdItemFromAddedModel(ERDItemModel* itemModel);
   void removeErdItemFromRemovedModel(ERDItemModel* itemModel);

   ERDModel* m_erdModel = nullptr;
   QMap<QString, Binding> m_idToBinding;

   AnchorItem* m_anchor;
};
