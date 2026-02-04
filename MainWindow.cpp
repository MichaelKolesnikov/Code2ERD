#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ERDScene.h"

#include "Items/LinkItem.h"
#include "Models/LinkItemModel.h"
#include "Models/PropertyItemModel.h"
#include "Items/PropertyItem.h"
#include "Items/LineItem.h"
#include "Models/LineItemModel.h"
#include "Items/LinkLineItem.h"
#include "Models/LinkLineItemModel.h"

MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent)
   , ui(new Ui::MainWindow)
{
   ui->setupUi(this);

   auto erdScene = new ERDScene(this);
   ui->graphicsView->setScene(erdScene);
   auto entityModel = new EntityItemModel(0, "Entity", {-200, -200});
   auto linkModel = new LinkItemModel(0, "Link", {200, 200});
   auto propertyModel = new PropertyItemModel(0, "Property", {200, -200});
   auto lineModel = new LineItemModel(0, {{0, 0}, {50, 50}, {0, -50}});
   auto linkLineModel = new LinkLineItemModel(0, {{0, 0}, {-50, -50}, {0, 50}}, 0, 0, "0", "1");

   erdScene->addItem(new LinkItem(linkModel));
   erdScene->addItem(new EntityItem(entityModel));
   erdScene->addItem(new PropertyItem(propertyModel));
   // erdScene->addItem(new LineItem(lineModel));
   erdScene->addItem(new LinkLineItem(linkLineModel));
}

MainWindow::~MainWindow()
{
   delete ui;
}

