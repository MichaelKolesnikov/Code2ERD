#include "ERDModel.h"

ERDModel::ERDModel()
{

}

void ERDModel::add(EntityModel *itemModel)
{
   m_entities.insert(itemModel);
}

void ERDModel::add(PropertyModel *itemModel)
{
   m_properties.insert(itemModel);
}

void ERDModel::add(LinkModel *itemModel)
{
   m_links.insert(itemModel);
}

void ERDModel::add(PropertyLineModel *itemModel)
{
   m_propertyLines.insert(itemModel);
}

void ERDModel::add(LinkLineModel *itemModel)
{
   m_linkLines.insert(itemModel);
}

void ERDModel::remove(EntityModel *itemModel)
{
   m_entities.remove(itemModel);
}

void ERDModel::remove(PropertyModel *itemModel)
{
   m_properties.remove(itemModel);
}

void ERDModel::remove(LinkModel *itemModel)
{
   m_links.remove(itemModel);
}

void ERDModel::remove(PropertyLineModel *itemModel)
{
   m_propertyLines.remove(itemModel);
}

void ERDModel::remove(LinkLineModel *itemModel)
{
   m_linkLines.remove(itemModel);
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

const QSet<PropertyLineModel *> &ERDModel::propertyLines() const
{
   return m_propertyLines;
}

const QSet<LinkLineModel *> &ERDModel::linkLines() const
{
   return m_linkLines;
}
