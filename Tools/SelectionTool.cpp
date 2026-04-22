#include "SelectionTool.h"
#include "Scene.h"
#include "Items/SelectionGroupItem.h"

#include <QGraphicsSceneContextMenuEvent>

SelectionTool::SelectionTool(Scene *diagramScene)
   : Tool(diagramScene)
   , m_mousePressEventPoint(std::nullopt)
   , m_selectionGroupItem(new SelectionGroupItem())
{
   m_scene->addItem(m_selectionGroupItem);
}

SelectionTool::~SelectionTool()
{
   delete m_selectionGroupItem;
}

void SelectionTool::activated()
{
}

void SelectionTool::deactivated()
{
   m_selectionGroupItem->hide();
}

const char* SelectionTool::nameToShow() const
{
   return "SelectionTool";
}

void SelectionTool::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
   if (!(event->button() & Qt::LeftButton))
   {
      return;
   }
   m_mousePressEventPoint = event->scenePos();
   m_scene->clearSelection();
   m_selectionGroupItem->setRect(QRectF(m_mousePressEventPoint.value(), m_mousePressEventPoint.value()));
   m_selectionGroupItem->show();
}

void SelectionTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
   if (m_mousePressEventPoint)
   {
      m_selectionGroupItem->setRect(QRectF(m_mousePressEventPoint.value(), event->scenePos()).normalized());
   }
   else
   {
      Tool::mouseMoveEvent(event);
   }
}

void SelectionTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
   if (!m_mousePressEventPoint)
   {
      return;
   }
   auto selectionRect = QRectF(m_mousePressEventPoint.value(), event->scenePos()).normalized();
   QList<QGraphicsItem*> intersectingItems = m_scene->items(selectionRect, Qt::IntersectsItemShape);
   for (QGraphicsItem* item : intersectingItems)
   {
      QRectF itemRect = item->boundingRect().translated(item->scenePos());
      if (selectionRect.contains(itemRect))
      {
         item->setSelected(true);
      }
   }
   // m_scene->updateSelection();
   m_mousePressEventPoint = std::nullopt;
   m_selectionGroupItem->hide();
}
