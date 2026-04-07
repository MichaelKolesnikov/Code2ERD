#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ERDScene.h"
#include "ERDSceneView.h"
#include "Mappers/ERDJsonMapper.h"
#include "Undo/ReplaceErdModelCommand.h"

#include <QFileDialog>
#include <QDir>
#include <QJsonDocument>
#include <QUndoStack>

bool saveTextFile(const QString &fileName, const QString &text)
{
   QFile file(fileName);
   if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
      return false;
   }

   QTextStream out(&file);
   out << text;

   file.close();
   return true;
}

MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent)
   , ui(new Ui::MainWindow)
{
   ui->setupUi(this);

   m_undoStack = new QUndoStack(this);

   m_erdScene = new ERDScene(this);
   m_erdScene->loadModel(new ERDModel());
   ui->graphicsView->init();
   ui->graphicsView->setScene(m_erdScene);

   connect(m_erdScene, &ERDScene::signalToPushCommand, this, [this](QUndoCommand* cmd){
      m_undoStack->push(cmd);
   });
}

MainWindow::~MainWindow()
{
   delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
   if (event->key() == Qt::Key_Z && (event->modifiers() & Qt::ControlModifier))
   {
      if (event->modifiers() & Qt::ShiftModifier)
      {
         m_undoStack->redo();
      }
      else
      {
         m_undoStack->undo();
      }
   }
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
   if (!erdModel)
   {
      return;
   }
   m_undoStack->push(new ReplaceErdModelCommand(m_erdScene->erdModel(), erdModel, m_erdScene));
   m_erdScene->loadModel(erdModel);
}

void MainWindow::on_actionSave_as_triggered()
{
   auto pathToFile = QFileDialog::getSaveFileName(this, "Save", QDir::homePath(), "*.json");
   auto jsonDoc = QJsonDocument(ERDMapper::toJson(m_erdScene->erdModel()));
   saveTextFile(pathToFile, QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Indented)));
}

