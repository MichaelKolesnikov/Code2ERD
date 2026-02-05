#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ERDScene.h"
#include "Mappers/ERDJsonMapper.h"

#include <QFileDialog>
#include <QDir>
#include <QJsonDocument>

MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent)
   , ui(new Ui::MainWindow)
{
   ui->setupUi(this);

   m_erdScene = new ERDScene(this);
   ui->graphicsView->setScene(m_erdScene);
}

MainWindow::~MainWindow()
{
   delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
   QString pathToFile = QFileDialog::getOpenFileName(this, "Open Json File", QDir::homePath(), "*.json");
   QFileInfo fileInto(pathToFile);
   if (!fileInto.exists())
   {
      return;
   }
   QFile file(pathToFile);
   if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
   {
      return;
   }
   QByteArray jsonData = file.readAll();
   file.close();
   QJsonParseError parseError;
   auto jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
   if (parseError.error != QJsonParseError::NoError)
   {
      return;
   }
   auto erdModel = ERDMapper::fromJson(jsonDoc.object());
   if (!erdModel.has_value())
   {
      return;
   }
   m_erdScene->loadModel(erdModel.value());
}

