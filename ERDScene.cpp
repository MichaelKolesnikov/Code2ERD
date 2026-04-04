#include "ERDScene.h"
#include "RestrictedMenu.h"
#include "Items/EntityItem.h"
#include "Items/LinkItem.h"
#include "Items/PropertyItem.h"
#include "Items/LinkLineItem.h"
#include "Undo/AddRemoveCommand.h"

#include <QGraphicsSceneContextMenuEvent>

void ERDScene::init()
{
   m_erdModel = new ERDModel();
   connect(m_erdModel, &ERDModel::added, this, [this](ERDItemModel* itemModel) {
      if (auto it = qobject_cast<LinkLineModel*>(itemModel))
      {
         addItem(new LinkLineItem(it));
      }
      else if (auto it = qobject_cast<LineModel*>(itemModel))
      {
         addItem(new LineItem(it));
      }
      else if (auto it = qobject_cast<LinkModel*>(itemModel))
      {
         addItem(new LinkItem(it));
      }
      else if (auto it = qobject_cast<PropertyModel*>(itemModel))
      {
         addItem(new PropertyItem(it));
      }
      else if (auto it = qobject_cast<EntityModel*>(itemModel))
      {
         addItem(new EntityItem(it));
      }
   });
   connect(m_erdModel, &ERDModel::removed, this, [this](ERDItemModel* itemModel) {
      for (auto item : items())
      {
         if (auto it = qgraphicsitem_cast<ERDItem*>(item))
         {
            if (it->id() == itemModel->id())
            {
               removeItem(it);
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
      addItem(new EntityItem(model));
   }
   for (auto model : erdModel->links())
   {
      addItem(new LinkItem(model));
   }
   for (auto model : erdModel->properties())
   {
      addItem(new PropertyItem(model));
   }
   for (auto model : erdModel->linkLines())
   {
      addItem(new LinkLineItem(model));
   }
   for (auto model : erdModel->propertyLines())
   {
      addItem(new LineItem(model));
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
      emit signalToPushCommand(new AddRemoveCommand(m_erdModel->getItemById(itemToRemove->id()), AddRemoveCommand::Remove, m_erdModel));
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
