#pragma once
#include <QtPlugin>

class IERDItem
{
public:
   virtual ~IERDItem() = default;

   virtual QString id() const = 0;
};

Q_DECLARE_INTERFACE(IERDItem, "com.Code2ERD.IERDItem")
