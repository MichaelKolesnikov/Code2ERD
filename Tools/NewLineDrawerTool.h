#pragma once
#include "Tool.h"
#include <QPointF>

class LineModel;
class LineItem;

class NewLineDrawerTool : public Tool
{
   Q_OBJECT
public:
   explicit NewLineDrawerTool(Scene *diagramScene);
   ~NewLineDrawerTool();
   virtual void activated() override;
   virtual void deactivated() override;
   virtual const char* nameToShow() const override;

   bool m_isFirstPartHorizontal = true;
   int m_bendNumber = 2;

protected:
   virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
   virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
   virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
   LineModel* m_lineModel;
   LineItem* m_lineItem;
   QPointF m_initialMouseScenePos;
};
