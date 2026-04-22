#pragma once
#include "Tool.h"
#include <QPointF>
#include <optional>

class SelectionGroupItem;

class SelectionTool : public Tool
{
   Q_OBJECT
public:
   explicit SelectionTool(Scene*);
   ~SelectionTool();

   virtual void activated() override;
   virtual void deactivated() override;
   virtual const char* nameToShow() const override;

protected:
   virtual void mousePressEvent(QGraphicsSceneMouseEvent*) override;
   virtual void mouseMoveEvent(QGraphicsSceneMouseEvent*) override;
   virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;

private:
   std::optional<QPointF> m_mousePressEventPoint;
   SelectionGroupItem* m_selectionGroupItem;
};
