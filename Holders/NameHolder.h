#pragma once
#include <QString>

class NameHolder
{
public:
   explicit NameHolder();
   explicit NameHolder(const QString&);

   QString name() const;
   void setName(const QString&);

private:
   QString m_name;
};

