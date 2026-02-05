#pragma once
#include <QGraphicsScene>
#include "Models/ERDModel.h"

class ERDScene : public QGraphicsScene
{
   Q_OBJECT

public:
   using QGraphicsScene::QGraphicsScene;

   void loadModel(ERDModel erdModel);
};
