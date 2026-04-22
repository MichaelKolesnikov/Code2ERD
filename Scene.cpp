#include "Scene.h"
#include "Tools/Tool.h"

#include "RestrictedMenu.h"

#include "Models/ERDModel.h"

#include "Items/EntityItem.h"
#include "Items/LinkLineItem.h"
#include "Items/AnchorItem.h"
#include "Items/SelectionGroupItem.h"

#include "Undo/AddRemoveCommand.h"

#include <QUuid>
#include <QGraphicsSceneContextMenuEvent>

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

void Scene::pushCommand(QUndoCommand *cmd)
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
   if (event->button() & Qt::RightButton)
   {
      return;
   }
   auto currentClampedItem = itemAt(event->scenePos(), QTransform());
   if (!currentClampedItem)
   {
      m_tool->mousePressEvent(event);
      return;
   }

   if (auto lineItem = qgraphicsitem_cast<LineItem*>(currentClampedItem))
   {
      // m_selectedLineManager->selectLineItem(lineItem);
   }
   else if (!selectedItems().empty() && (m_anchor = qgraphicsitem_cast<AnchorItem*>(currentClampedItem)))
   {
//      if (m_selectedLineManager->hasLine())
//      {
//         m_selectedLineManager->prepareForAnchorMoving(m_anchor);
//         event->accept();
//      }
//      else
//      {
//         m_workWithSelectionGroupItem = true;
//         m_selectionGroupManager->prepareForResizing(m_anchor, event->scenePos());
//         event->accept();
//      }
   }
   else if (currentClampedItem->type() == EntityItem::Type)
   {
//      m_selectedLineManager->removeLineItemSelection();
//      if (!(event->modifiers() & Qt::ControlModifier))
//      {
//         clearSelection();
//      }
//      currentClampedItem->setSelected(true);
//      m_selectionGroupManager->updateSelectionGroup();
//      m_selectionGroupManager->prepareForMoving(event->scenePos());
//      m_workWithSelectionGroupItem = true;
   }
   else if (currentClampedItem->type() == SelectionGroupItem::Type)
   {
      // m_workWithSelectionGroupItem = true;
      // m_selectionGroupManager->prepareForMoving(event->scenePos());
   }
   else
   {
      clearSelection();
      // m_selectionGroupManager->updateSelectionGroup();
      m_tool->mousePressEvent(event);
   }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
   m_tool->mouseMoveEvent(event);
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
   m_tool->mouseReleaseEvent(event);
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
