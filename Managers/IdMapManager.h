#pragma once
#include <QMap>
#include <Models/ERDItemModel.h>
#include <Items/ERDItem.h>

struct Binding
{
   ERDItemModel* erdItemModel;
   ERDItem* erdItem;
};

class IdMapManager : public QMap<int, Binding>
{
public:
   using QMap<int, Binding>::QMap;
};
