#include "ERDModel.h"

ERDModel::ERDModel()
{

}

void ERDModel::add(ERDItemModel *itemModel)
{
   if (auto it = qobject_cast<LinkLineModel*>(itemModel))
   {
      add(it);
   }
   else if (auto it = qobject_cast<LineModel*>(itemModel))
   {
      add(it);
   }
   else if (auto it = qobject_cast<LinkModel*>(itemModel))
   {
      add(it);
   }
   else if (auto it = qobject_cast<PropertyModel*>(itemModel))
   {
      add(it);
   }
   else if (auto it = qobject_cast<EntityModel*>(itemModel))
   {
      add(it);
   }
}

void ERDModel::add(EntityModel *itemModel)
{
   m_entities[itemModel->id()] = itemModel;
   emit added(itemModel);
}

void ERDModel::add(PropertyModel *itemModel)
{
   m_properties[itemModel->id()] = itemModel;
   emit added(itemModel);
}

void ERDModel::add(LinkModel *itemModel)
{
   m_links[itemModel->id()] = itemModel;
   emit added(itemModel);
}

void ERDModel::add(LineModel *itemModel)
{
   m_propertyLines[itemModel->id()] = itemModel;
   emit added(itemModel);
}

void ERDModel::add(LinkLineModel *itemModel)
{
   m_linkLines[itemModel->id()] = itemModel;
   emit added(itemModel);
}

void ERDModel::set(LinesConnectionsModel *linesConnectionsModel)
{
   if (m_linesConnectionsModel != linesConnectionsModel)
   {
      m_linesConnectionsModel = linesConnectionsModel;
      emit seted(linesConnectionsModel);
   }
}

void ERDModel::remove(ERDItemModel *itemModel)
{
   if (auto it = qobject_cast<LinkLineModel*>(itemModel))
   {
      remove(it);
   }
   else if (auto it = qobject_cast<LineModel*>(itemModel))
   {
      remove(it);
   }
   else if (auto it = qobject_cast<LinkModel*>(itemModel))
   {
      remove(it);
   }
   else if (auto it = qobject_cast<PropertyModel*>(itemModel))
   {
      remove(it);
   }
   else if (auto it = qobject_cast<EntityModel*>(itemModel))
   {
      remove(it);
   }
}

void ERDModel::remove(EntityModel *itemModel)
{
   m_entities.remove(itemModel->id());
   emit removed(itemModel);
}

void ERDModel::remove(PropertyModel *itemModel)
{
   m_properties.remove(itemModel->id());
   emit removed(itemModel);
}

void ERDModel::remove(LinkModel *itemModel)
{
   m_links.remove(itemModel->id());
   emit removed(itemModel);
}

void ERDModel::remove(LineModel *itemModel)
{
   m_propertyLines.remove(itemModel->id());
   emit removed(itemModel);
}

void ERDModel::remove(LinkLineModel *itemModel)
{
   m_linkLines.remove(itemModel->id());
   emit removed(itemModel);
}

QList<EntityModel *> ERDModel::entities() const
{
   return m_entities.values();
}

QList<PropertyModel *> ERDModel::properties() const
{
   return m_properties.values();
}

QList<LinkModel *> ERDModel::links() const
{
   return m_links.values();
}

QList<LineModel *> ERDModel::propertyLines() const
{
   return m_propertyLines.values();
}

QList<LinkLineModel *> ERDModel::linkLines() const
{
   return m_linkLines.values();
}

ERDItemModel *ERDModel::getItemById(int id)
{
   if (m_entities.contains(id))
   {
      return m_entities[id];
   }
   else if (m_properties.contains(id))
   {
      return m_properties[id];
   }
   else if (m_links.contains(id))
   {
      return m_links[id];
   }
   else if (m_propertyLines.contains(id))
   {
      return m_propertyLines[id];
   }
   else if (m_linkLines.contains(id))
   {
      return m_linkLines[id];
   }
   return nullptr;
}
