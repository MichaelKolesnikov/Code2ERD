#include "ERDItemModel.h"

ERDItemModel::ERDItemModel(int id) : QObject(), m_id(id)
{

}

int ERDItemModel::id() const
{
   return m_id;
}
