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
   m_entities.insert(itemModel);
   emit added(itemModel);
}

void ERDModel::add(PropertyModel *itemModel)
{
   m_properties.insert(itemModel);
   emit added(itemModel);
}

void ERDModel::add(LinkModel *itemModel)
{
   m_links.insert(itemModel);
   emit added(itemModel);
}

void ERDModel::add(LineModel *itemModel)
{
   m_propertyLines.insert(itemModel);
   emit added(itemModel);
}

void ERDModel::add(LinkLineModel *itemModel)
{
   m_linkLines.insert(itemModel);
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
   m_entities.remove(itemModel);
   emit removed(itemModel);
}

void ERDModel::remove(PropertyModel *itemModel)
{
   m_properties.remove(itemModel);
   emit removed(itemModel);
}

void ERDModel::remove(LinkModel *itemModel)
{
   m_links.remove(itemModel);
   emit removed(itemModel);
}

void ERDModel::remove(LineModel *itemModel)
{
   m_propertyLines.remove(itemModel);
   emit removed(itemModel);
}

void ERDModel::remove(LinkLineModel *itemModel)
{
   m_linkLines.remove(itemModel);
   emit removed(itemModel);
}

const QSet<EntityModel *> &ERDModel::entities() const
{
   return m_entities;
}

const QSet<PropertyModel *> &ERDModel::properties() const
{
   return m_properties;
}

const QSet<LinkModel *> &ERDModel::links() const
{
   return m_links;
}

const QSet<LineModel *> &ERDModel::propertyLines() const
{
   return m_propertyLines;
}

const QSet<LinkLineModel *> &ERDModel::linkLines() const
{
   return m_linkLines;
}
