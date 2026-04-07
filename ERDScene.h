#pragma once
#include <QGraphicsScene>
#include <QMap>

class QUndoCommand;
class ERDItemModel;
class ERDModel;
class ERDItem;

class ERDScene : public QGraphicsScene
{
   Q_OBJECT

public:
   using QGraphicsScene::QGraphicsScene;

   void loadModel(ERDModel* erdModel);

   ERDModel* erdModel();

signals:
   void signalToPushCommand(QUndoCommand*);

protected:
   virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
   virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
   virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
   virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
   struct Binding
   {
      ERDItemModel* erdItemModel;
      ERDItem* erdItem;
   };

   void addErdItemFromAddedModel(ERDItemModel* itemModel);
   void removeErdItemFromRemovedModel(ERDItemModel* itemModel);

   ERDModel* m_erdModel = nullptr;
   QMap<QString, Binding> m_idToBinding;
};
