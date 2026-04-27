#include "SelectedLineManager.h"
#include "Scene.h"
#include "Models/LineModel.h"
#include "Items/LineItem.h"
#include "Items/GhostLineItem.h"
#include "Items/AnchorItem.h"
#include "ObjectPool.h"
#include "LineGeometryManager.h"

#include "Undo/ChangeLineNodesCommand.h"

#include <QGraphicsColorizeEffect>
#include <QGraphicsSceneMouseEvent>

SelectedLineManager::SelectedLineManager(Scene* scene, const QMap<QString, Binding>& idToBinding)
   : QObject(scene)
   , m_idToBinding(idToBinding)
   , m_diagramScene(scene)
   , m_anchorItemObjectPool(new ObjectPool<AnchorItem>())
   , m_selectedLine(nullptr)
   , m_tmpLine(new GhostLineItem())
   , m_draggedAnchor(nullptr)
   , m_initialDraggedAnchorScenePos(QPointF(0, 0))
{
   m_diagramScene->addItem(m_tmpLine);
   m_tmpLine->hide();
}

SelectedLineManager::~SelectedLineManager()
{
}

void SelectedLineManager::selectLineItem(LineItem *lineItem)
{
   removeLineItemSelection();
   m_selectedLine = lineItem;
   if (!m_selectedLine)
   {
      return;
   }
   lineItem->setSelected(true);
   setAnchorsOnLineItem(lineItem);
}

void SelectedLineManager::removeLineItemSelection()
{
   if (m_selectedLine)
   {
      m_selectedLine->setSelected(false);
   }
   for (auto anchor : m_anchors)
   {
      m_diagramScene->removeItem(anchor);
      m_anchorItemObjectPool->release(anchor);
   }
   m_anchors.clear();
   m_selectedLine = nullptr;
}

void SelectedLineManager::prepareForAnchorMoving(AnchorItem *currentDraggedAnchor)
{
   m_tmpLine->show();
   m_tmpLine->setNodes(m_selectedLine->nodes());
   m_draggedAnchor = currentDraggedAnchor;
   m_initialDraggedAnchorScenePos = currentDraggedAnchor->scenePos();
}

void SelectedLineManager::moveChosenAnchor(QGraphicsSceneMouseEvent *event)
{
   setNewPosToAnchorWithLineNode(event->scenePos());
}

void SelectedLineManager::finishToMoveChosenAnchor(QGraphicsSceneMouseEvent *event)
{
   setNewPosToAnchorWithLineNode(event->scenePos());
   m_draggedAnchor = nullptr;
   m_tmpLine->hide();

   if (auto lineModel = qobject_cast<LineModel*>(m_idToBinding[m_selectedLine->id()].erdItemModel))
   {
      m_diagramScene->pushCommand(new ChangeLineNodesCommand(lineModel, m_tmpLine->nodes()));
   }
   auto it = m_selectedLine;
   removeLineItemSelection();
   selectLineItem(it);
}

bool SelectedLineManager::hasLine() const
{
   return m_selectedLine;
}

void SelectedLineManager::setNewPosToAnchorWithLineNode(QPointF mousePos)
{
   QPointF delta = mousePos - m_initialDraggedAnchorScenePos;
   auto constraint = m_draggedAnchor->constraintType();
   QPointF constraintDirection(0, 0);
   if (constraint == AnchorItem::Horizontal)
   {
      constraintDirection = {1, 0};
   }
   else if (constraint == AnchorItem::Vertical)
   {
      constraintDirection = {0, 1};
   }
   else
   {
      m_draggedAnchor->setPos(mousePos);
      m_tmpLine->setNodes(LineGeometryManager::updateNode(m_selectedLine->nodes(), m_anchors.indexOf(m_draggedAnchor), m_draggedAnchor->scenePos(), m_freeNodesEditing));
      return;
   }
   qreal directionLengthSquared = constraintDirection.x() * constraintDirection.x() + constraintDirection.y() * constraintDirection.y();
   if (directionLengthSquared == 0)
   {
      return;
   }
   qreal dotProduct = delta.x() * constraintDirection.x() + delta.y() * constraintDirection.y();
   qreal projectionFactor = dotProduct / directionLengthSquared;
   QPointF projectedDelta(projectionFactor * constraintDirection.x(), projectionFactor * constraintDirection.y());
   QPointF newAnchorScenePos = m_initialDraggedAnchorScenePos + projectedDelta;
   m_draggedAnchor->setPos(newAnchorScenePos);
   m_tmpLine->setNodes(LineGeometryManager::updateNode(m_selectedLine->nodes(), m_anchors.indexOf(m_draggedAnchor), m_draggedAnchor->scenePos(), m_freeNodesEditing));
}

void SelectedLineManager::setAnchorsOnLineItem(LineItem *lineItem)
{
   m_anchors.clear();
   auto points = lineItem->nodes();
   for (auto point : points)
   {
      auto anchorItem = m_anchorItemObjectPool->get(AnchorItem::NoConstraint);
      m_diagramScene->addItem(anchorItem);
      anchorItem->setPos(point);
      m_anchors.append(anchorItem);
   }
}

bool SelectedLineManager::isStartAnchorChosen() const
{
   return m_anchors.indexOf(m_draggedAnchor) == 0;
}

bool SelectedLineManager::isEndAnchorChosen() const
{
   return m_anchors.indexOf(m_draggedAnchor) == m_anchors.size() - 1;
}
