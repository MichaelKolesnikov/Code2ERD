#pragma once
#include <QGraphicsScene>
#include "Models/ERDModel.h"

class QUndoCommand;

class ERDScene : public QGraphicsScene
{
   Q_OBJECT

public:
   using QGraphicsScene::QGraphicsScene;

   void init();

   void loadModel(ERDModel* erdModel);

signals:
   void signalToPushCommand(QUndoCommand*);

protected:
   virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
   virtual void keyReleaseEvent(QKeyEvent *event) override;
   virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
   virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
   virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
   virtual void wheelEvent(QGraphicsSceneWheelEvent *event) override;

private:
   ERDModel* m_erdModel;
};
