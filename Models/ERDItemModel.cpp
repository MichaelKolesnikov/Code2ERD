#include "ERDItemModel.h"

ERDItemModel::ERDItemModel(QString id) : QObject(), m_id(id)
{

}

QString ERDItemModel::id() const
{
   return m_id;
}
