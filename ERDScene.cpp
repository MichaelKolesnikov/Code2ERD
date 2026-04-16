#include "ERDScene.h"
#include "RestrictedMenu.h"
#include "Models/ERDModel.h"
#include "Items/EntityItem.h"
#include "Items/LinkLineItem.h"
#include "Undo/AddRemoveCommand.h"

#include <QUuid>
#include <QGraphicsSceneContextMenuEvent>

void ERDScene::loadModel(ERDModel* erdModel)
{
   if (m_erdModel)
   {
      for (auto i : m_idToBinding.keys())
      {
         removeItem(m_idToBinding[i].erdItem);
      }
      m_idToBinding.clear();
      disconnect(m_erdModel, &ERDModel::added, this, &ERDScene::addErdItemFromAddedModel);
      disconnect(m_erdModel, &ERDModel::removed, this, &ERDScene::removeErdItemFromRemovedModel);
   }

   m_erdModel = erdModel;

   if (!m_erdModel)
   {
      return;
   }

   connect(m_erdModel, &ERDModel::added, this, &ERDScene::addErdItemFromAddedModel);
   connect(m_erdModel, &ERDModel::removed, this, &ERDScene::removeErdItemFromRemovedModel);
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

ERDModel *ERDScene::erdModel()
{
   return m_erdModel;
}

void ERDScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
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

void ERDScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}

void ERDScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

}

void ERDScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

}

void ERDScene::addErdItemFromAddedModel(ERDItemModel *itemModel)
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

void ERDScene::removeErdItemFromRemovedModel(ERDItemModel *itemModel)
{
   removeItem(m_idToBinding[itemModel->id()].erdItem);
   m_idToBinding.remove(itemModel->id());
}
