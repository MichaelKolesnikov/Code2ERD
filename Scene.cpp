#include "Scene.h"
#include "Tools/Tool.h"

#include "RestrictedMenu.h"

#include "Models/ERDModel.h"

#include "Items/EntityItem.h"
#include "Items/LinkLineItem.h"
#include "Items/AnchorItem.h"
#include "Items/SelectionGroupItem.h"

#include "Managers/SelectedLineManager.h"

#include "Undo/AddRemoveCommand.h"

#include <QUuid>
#include <QGraphicsSceneContextMenuEvent>
#include <QPainter>

void Scene::init()
{
   if (m_isInited)
   {
      return;
   }
   m_isInited = true;
   setSceneRect(-2000, -2000, 4000, 4000);
   m_selectedLineManager = new SelectedLineManager(this, m_idToBinding);
}

void Scene::loadModel(ERDModel* erdModel)
{
   if (m_erdModel)
   {
      for (auto i : m_idToBinding.keys())
      {
         removeItem(m_idToBinding[i].erdItem);
      }
      m_idToBinding.clear();
      disconnect(m_erdModel, &ERDModel::added, this, &Scene::addErdItemFromAddedModel);
      disconnect(m_erdModel, &ERDModel::removed, this, &Scene::removeErdItemFromRemovedModel);
   }

   m_erdModel = erdModel;

   if (!m_erdModel)
   {
      return;
   }

   connect(m_erdModel, &ERDModel::added, this, &Scene::addErdItemFromAddedModel);
   connect(m_erdModel, &ERDModel::removed, this, &Scene::removeErdItemFromRemovedModel);
   for (auto model : erdModel->entities())
   {
      addErdItemFromAddedModel(model);
   }
   for (auto model : erdModel->linkLines())
   {
      addErdItemFromAddedModel(model);
   }
   for (auto model : erdModel->propertyLines())
   {
      addErdItemFromAddedModel(model);
   }
}

ERDModel *Scene::erdModel()
{
   return m_erdModel;
}

void Scene::pushCommand(UndoCommand *cmd)
{
   emit signalToPushCommand(cmd);
}

const Tool *Scene::currentTool() const
{
   return m_tool;
}

void Scene::setTool(Tool *tool)
{
   if (m_tool == tool)
   {
      return;
   }
   if (m_tool)
   {
      m_tool->deactivated();
   }
   m_tool = tool;
   m_tool->activated();
}

void Scene::customClearSelection()
{
   m_selectedLineManager->removeLineItemSelection();
   clearSelection();
}

void Scene::setFreeNodesEditing(bool free)
{
   m_selectedLineManager->m_freeNodesEditing = free;
}

void Scene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
   RestrictedMenu menu;

   QAction* addEntityAction = nullptr;
   QAction* addLinkAction = nullptr;
   QAction* addPropertyAction = nullptr;
   QAction* removeAction = nullptr;
   ERDItem* itemToRemove = nullptr;

   auto itemsUnderCursor = items(event->scenePos());
   if (itemsUnderCursor.empty())
   {
      addEntityAction = menu.addAction("Add entity");
      addLinkAction = menu.addAction("Add link");
      addPropertyAction = menu.addAction("Add property");
   }
   for (auto item : itemsUnderCursor)
   {
      if (item)
      {
         removeAction = menu.addAction("Remove");
         itemToRemove = qgraphicsitem_cast<ERDItem*>(item);
      }
   }

   auto selectedAction = menu.exec(event->screenPos());
   if (!selectedAction)
   {
      return;
   }

   QSizeF size(50, 30);
   if (selectedAction == addEntityAction)
   {
      emit signalToPushCommand(new AddRemoveCommand(new IdNamePositionSizeTypeModel({{QUuid::createUuid().toString()}, "Entity", event->scenePos(), size, IdNamePositionSizeTypeDTO::Entity}), AddRemoveCommand::Add, m_erdModel));
   }
   else if (selectedAction == addLinkAction)
   {
      emit signalToPushCommand(new AddRemoveCommand(new IdNamePositionSizeTypeModel({{QUuid::createUuid().toString()}, "Link", event->scenePos(), size, IdNamePositionSizeTypeDTO::Link}), AddRemoveCommand::Add, m_erdModel));
   }
   else if (selectedAction == addPropertyAction)
   {
      emit signalToPushCommand(new AddRemoveCommand(new IdNamePositionSizeTypeModel({{QUuid::createUuid().toString()}, "Property", event->scenePos(), size, IdNamePositionSizeTypeDTO::Property}), AddRemoveCommand::Add, m_erdModel));
   }
   else if (selectedAction == removeAction && itemToRemove)
   {
      emit signalToPushCommand(new AddRemoveCommand(m_idToBinding[itemToRemove->id()].erdItemModel, AddRemoveCommand::Remove, m_erdModel));
   }
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
   if (event->button() == Qt::RightButton)
   {
      m_toTool = false;
      m_wasPressed = false;
      return;
   }
   auto currentClampedItem = itemAt(event->scenePos(), QTransform());
   if (!currentClampedItem)
   {
      customClearSelection();
      m_tool->mousePressEvent(event);
      m_toTool = true;
      m_wasPressed = true;
   }
   else
   {
      m_wasPressed = true;
      m_toTool = false;
      auto selected = selectedItems();

      if (auto lineItem = qgraphicsitem_cast<LineItem*>(currentClampedItem))
      {
         customClearSelection();
         m_selectedLineManager->selectLineItem(lineItem);
         auto selected = selectedItems();
      }
      else if (!selected.empty() && (m_anchor = qgraphicsitem_cast<AnchorItem*>(currentClampedItem)))
      {
         if (m_selectedLineManager->hasLine())
         {
            m_selectedLineManager->prepareForAnchorMoving(m_anchor, event->modifiers() & Qt::ControlModifier);
            event->accept();
         }
      }
      else if (currentClampedItem->type() == EntityItem::Type)
      {
         customClearSelection();
      }
      else if (currentClampedItem->type() == SelectionGroupItem::Type)
      {
      }
      else
      {
         customClearSelection();
         m_tool->mousePressEvent(event);
      }
   }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
   if (m_wasPressed == true)
   {
      auto selected = selectedItems();
      if (m_toTool)
      {
         m_tool->mouseMoveEvent(event);
         return;
      }
      if (m_selectedLineManager->hasLine())
      {
         if (m_anchor)
         {
            m_selectedLineManager->moveChosenAnchor(event);
         }
         QGraphicsScene::mouseMoveEvent(event);
         return;
      }
   }
   else
   {
      QGraphicsScene::mouseMoveEvent(event);
   }
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
   if (m_wasPressed)
   {
      m_wasPressed = false;
      if (m_toTool)
      {
         m_tool->mouseReleaseEvent(event);
         return;
      }
      if (m_selectedLineManager->hasLine())
      {
         if (m_anchor)
         {
            m_selectedLineManager->finishToMoveChosenAnchor(event);
            m_anchor = nullptr;
         }
         auto selected = selectedItems();
         return;
      }
      m_anchor = nullptr;
   }
}

void Scene::drawBackground(QPainter *painter, const QRectF &rect)
{
   static const int gridSize = 15;
   painter->save();

   painter->fillRect(rect, Qt::lightGray);
   painter->fillRect(sceneRect(), Qt::white);

   painter->setClipRect(sceneRect());

   QPen gridPen(QColor(240, 240, 240), 1.0);
   gridPen.setCosmetic(true);
   painter->setPen(gridPen);

   int left = int(sceneRect().left() / gridSize) * gridSize;
   int right = sceneRect().right();
   int top = sceneRect().top();
   int bottom = sceneRect().bottom();
   for (auto x = left; x < right; x += gridSize)
   {
      painter->drawLine(QPointF(x, top), QPointF(x, bottom));
   }
   for (auto y = top; y < bottom; y += gridSize)
   {
      painter->drawLine(QPointF(left, y), QPointF(right, y));
   }

   painter->restore();
}

void Scene::addErdItemFromAddedModel(ERDItemModel *itemModel)
{
   if (auto it = qobject_cast<LinkLineModel*>(itemModel))
   {
      auto item = new LinkLineItem(it);
      m_idToBinding[itemModel->id()] = {itemModel, item};
      addItem(item);
   }
   else if (auto it = qobject_cast<LineModel*>(itemModel))
   {
      auto item = new LineItem(it);
      m_idToBinding[itemModel->id()] = {itemModel, item};
      addItem(item);
   }
   else if (auto it = qobject_cast<IdNamePositionSizeTypeModel*>(itemModel))
   {
      auto item = new EntityItem(it);
      m_idToBinding[itemModel->id()] = {itemModel, item};
      addItem(item);
   }
}

void Scene::removeErdItemFromRemovedModel(ERDItemModel *itemModel)
{
   removeItem(m_idToBinding[itemModel->id()].erdItem);
   m_idToBinding.remove(itemModel->id());
}

// base
void Scene::baseContextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
   QGraphicsScene::contextMenuEvent(event);
}
void Scene::baseKeyPressEvent(QKeyEvent *event)
{
   QGraphicsScene::keyPressEvent(event);
}
void Scene::baseKeyReleaseEvent(QKeyEvent *event)
{
   QGraphicsScene::keyReleaseEvent(event);
}
void Scene::baseMousePressEvent(QGraphicsSceneMouseEvent *event)
{
   QGraphicsScene::mousePressEvent(event);
}
void Scene::baseMouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
   QGraphicsScene::mouseMoveEvent(event);
}
void Scene::baseMouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
   QGraphicsScene::mouseReleaseEvent(event);
}
void Scene::baseMouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
   QGraphicsScene::mouseDoubleClickEvent(event);
}
void Scene::baseWheelEvent(QGraphicsSceneWheelEvent *event)
{
   QGraphicsScene::wheelEvent(event);
}
