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
   else if (auto it = qobject_cast<IdNamePositionSizeTypeModel*>(itemModel))
   {
      add(it);
   }
}

void ERDModel::add(IdNamePositionSizeTypeModel *itemModel)
{
   m_entities.insert(itemModel);
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
   else if (auto it = qobject_cast<IdNamePositionSizeTypeModel*>(itemModel))
   {
      remove(it);
   }
}

void ERDModel::remove(IdNamePositionSizeTypeModel *itemModel)
{
   m_entities.remove(itemModel);
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

const QSet<IdNamePositionSizeTypeModel *> &ERDModel::entities() const
{
   return m_entities;
}

const QSet<LineModel *> &ERDModel::propertyLines() const
{
   return m_propertyLines;
}

const QSet<LinkLineModel *> &ERDModel::linkLines() const
{
   return m_linkLines;
}

void ERDModel::add(const IdNamePositionSizeTypeDTO& dto)
{
   auto model = new IdNamePositionSizeTypeModel(dto);
   add(model);
}

void ERDModel::add(const LineDTO& dto)
{
   auto model = new LineModel(dto);
   add(model);
}

void ERDModel::add(const LinkLineDTO& dto)
{
   auto model = new LinkLineModel(dto);
   add(model);
}
