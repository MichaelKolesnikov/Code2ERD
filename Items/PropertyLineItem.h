#pragma once
#include "LineItem.h"
class PropertyLineItemModel;

class PropertyLineItem : public LineItem
{
   Q_OBJECT
public:
   explicit PropertyLineItem(PropertyLineItemModel*);

private:
   PropertyLineItemModel* m_model;
};

