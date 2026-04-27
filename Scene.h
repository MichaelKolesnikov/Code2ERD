#pragma once
#include <QGraphicsScene>
#include <QMap>
#include "ModelItemBinding.h"

class QUndoCommand;
class ERDItemModel;
class ERDModel;
class ERDItem;
class AnchorItem;
class Tool;
class SelectedLineManager;

class Scene : public QGraphicsScene
{
   Q_OBJECT

public:
   using QGraphicsScene::QGraphicsScene;

   void init();

   void loadModel(ERDModel* erdModel);

   ERDModel* erdModel();

   void pushCommand(QUndoCommand*);

   const Tool* currentTool() const;
   void setTool(Tool*);

   void customClearSelection();

   void setFreeNodesEditing(bool free);

signals:
   void signalToPushCommand(QUndoCommand*);

protected:
   virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
   virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
   virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
   virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
   virtual void drawBackground(QPainter *painter, const QRectF& rect) override;

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
   bool m_isInited = false;

   Tool* m_tool = nullptr;

   void addErdItemFromAddedModel(ERDItemModel* itemModel);
   void removeErdItemFromRemovedModel(ERDItemModel* itemModel);

   ERDModel* m_erdModel = nullptr;
   QMap<QString, Binding> m_idToBinding;

   AnchorItem* m_anchor = nullptr;

   SelectedLineManager* m_selectedLineManager;

   bool m_wasPressed = false;
   bool m_toTool = false;
};
