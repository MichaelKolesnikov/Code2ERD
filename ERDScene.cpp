#include "ERDScene.h"
#include "Items/EntityItem.h"
#include "Items/LinkItem.h"
#include "Items/PropertyItem.h"
#include "Items/LinkLineItem.h"
#include "Items/PropertyLineItem.h"

void ERDScene::loadModel(ERDModel erdModel)
{
   for (auto model : erdModel.entities())
   {
      addItem(new EntityItem(model));
   }
   for (auto model : erdModel.links())
   {
      addItem(new LinkItem(model));
   }
   for (auto model : erdModel.properties())
   {
      addItem(new PropertyItem(model));
   }
   for (auto model : erdModel.linkLines())
   {
      addItem(new LinkLineItem(model));
   }
   for (auto model : erdModel.propertyLines())
   {
      addItem(new PropertyLineItem(model));
   }
}
