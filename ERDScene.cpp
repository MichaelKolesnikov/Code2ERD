#include "ERDScene.h"
#include "RestrictedMenu.h"
#include "Models/ERDModel.h"
#include "Items/EntityItem.h"
#include "Items/LinkItem.h"
#include "Items/PropertyItem.h"
#include "Items/LinkLineItem.h"
#include "Undo/AddRemoveCommand.h"

#include <QGraphicsSceneContextMenuEvent>

void ERDScene::init()
{
   m_erdModel = new ERDModel();
   connect(m_erdModel, &ERDModel::added, this, &ERDScene::addErdItemFromModel);
   connect(m_erdModel, &ERDModel::removed, this, [this](ERDItemModel* itemModel) {
      for (auto item : items())
      {
         if (auto it = qgraphicsitem_cast<ERDItem*>(item))
         {
            if (it->id() == itemModel->id())
            {
               removeItem(it);
               m_idToBinding.remove(it->id());
               break;
            }
         }
      }
   });
}

void ERDScene::loadModel(ERDModel* erdModel)
{
   clear();
   for (auto model : erdModel->entities())
   {
      addErdItemFromModel(model);
   }
   for (auto model : erdModel->links())
   {
      addErdItemFromModel(model);
   }
   for (auto model : erdModel->properties())
   {
      addErdItemFromModel(model);
   }
   for (auto model : erdModel->linkLines())
   {
      addErdItemFromModel(model);
   }
   for (auto model : erdModel->propertyLines())
   {
      addErdItemFromModel(model);
   }
   m_erdModel = erdModel;
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

   if (selectedAction == addEntityAction)
   {
      emit signalToPushCommand(new AddRemoveCommand(new EntityModel(0, "Entity", event->scenePos()), AddRemoveCommand::Add, m_erdModel));
   }
   else if (selectedAction == addLinkAction)
   {
      emit signalToPushCommand(new AddRemoveCommand(new LinkModel(0, "Link", event->scenePos()), AddRemoveCommand::Add, m_erdModel));
   }
   else if (selectedAction == addPropertyAction)
   {
      emit signalToPushCommand(new AddRemoveCommand(new PropertyModel(0, "Property", event->scenePos()), AddRemoveCommand::Add, m_erdModel));
   }
   else if (selectedAction == removeAction && itemToRemove)
   {
      emit signalToPushCommand(new AddRemoveCommand(m_idToBinding[itemToRemove->id()].erdItemModel, AddRemoveCommand::Remove, m_erdModel));
   }
}

void ERDScene::keyReleaseEvent(QKeyEvent *event)
{

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

void ERDScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{

}

void ERDScene::addErdItemFromModel(ERDItemModel *itemModel)
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
   else if (auto it = qobject_cast<LinkModel*>(itemModel))
   {
      auto item = new LinkItem(it);
      m_idToBinding[itemModel->id()] = {itemModel, item};
      addItem(item);
   }
   else if (auto it = qobject_cast<PropertyModel*>(itemModel))
   {
      auto item = new PropertyItem(it);
      m_idToBinding[itemModel->id()] = {itemModel, item};
      addItem(item);
   }
   else if (auto it = qobject_cast<EntityModel*>(itemModel))
   {
      auto item = new EntityItem(it);
      m_idToBinding[itemModel->id()] = {itemModel, item};
      addItem(item);
   }
}
