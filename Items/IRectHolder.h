#pragma once
#include <QtPlugin>

class IRectHolder
{
public:
   virtual QRectF boundingRect() const = 0;
   virtual void setRect(QRectF) = 0;
};

Q_DECLARE_INTERFACE(IRectHolder, "com.Logos.IRectHolder/1.0")
