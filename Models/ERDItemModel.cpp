#include "ERDItemModel.h"

ERDItemModel::ERDItemModel(int id) : m_id(id)
{

}

int ERDItemModel::id() const
{
   return m_id;
}
