#pragma once
#include "LineItem.h"
class PropertyLineModel;

class PropertyLineItem : public LineItem
{
   Q_OBJECT
public:
   explicit PropertyLineItem(PropertyLineModel*);

private:
   PropertyLineModel* m_model;
};

