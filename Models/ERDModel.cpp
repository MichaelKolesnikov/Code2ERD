#include "ERDModel.h"

ERDModel::ERDModel()
{

}

void ERDModel::add(EntityItemModel *itemModel)
{
   m_entities.insert(itemModel);
}

void ERDModel::add(PropertyItemModel *itemModel)
{
   m_properties.insert(itemModel);
}

void ERDModel::add(LinkItemModel *itemModel)
{
   m_links.insert(itemModel);
}

void ERDModel::add(PropertyLineItemModel *itemModel)
{
   m_propertyLines.insert(itemModel);
}

void ERDModel::add(LinkLineItemModel *itemModel)
{
   m_linkLines.insert(itemModel);
}

void ERDModel::remove(EntityItemModel *itemModel)
{
   m_entities.remove(itemModel);
}

void ERDModel::remove(PropertyItemModel *itemModel)
{
   m_properties.remove(itemModel);
}

void ERDModel::remove(LinkItemModel *itemModel)
{
   m_links.remove(itemModel);
}

void ERDModel::remove(PropertyLineItemModel *itemModel)
{
   m_propertyLines.remove(itemModel);
}

void ERDModel::remove(LinkLineItemModel *itemModel)
{
   m_linkLines.remove(itemModel);
}

const QSet<EntityItemModel *> &ERDModel::entities() const
{
   return m_entities;
}

const QSet<PropertyItemModel *> &ERDModel::properties() const
{
   return m_properties;
}

const QSet<LinkItemModel *> &ERDModel::links() const
{
   return m_links;
}

const QSet<PropertyLineItemModel *> &ERDModel::propertyLines() const
{
   return m_propertyLines;
}

const QSet<LinkLineItemModel *> &ERDModel::linkLines() const
{
   return m_linkLines;
}
