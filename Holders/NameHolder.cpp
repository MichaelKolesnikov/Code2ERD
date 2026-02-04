#include "NameHolder.h"

NameHolder::NameHolder() : m_name("")
{

}

NameHolder::NameHolder(const QString &name) : m_name(name)
{

}

QString NameHolder::name() const
{
   return m_name;
}

void NameHolder::setName(const QString &name)
{
   m_name = name;
}
