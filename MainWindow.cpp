#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Scene.h"
#include "SceneView.h"
#include "Mappers/ERDJsonMapper.h"
#include "Undo/ReplaceErdModelCommand.h"

#include "Tools/SelectionTool.h"
#include "Tools/NewLineDrawerTool.h"

#include <QFileDialog>
#include <QDir>
#include <QJsonDocument>
#include <QUndoStack>
#include <QToolBar>

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
   ui->undoView->setStack(m_undoStack);

   m_scene = new Scene(this);
   m_scene->loadModel(new ERDModel());
   ui->graphicsView->init();
   ui->graphicsView->setScene(m_scene);

   connect(m_scene, &Scene::signalToPushCommand, this, [this](QUndoCommand* cmd){
      m_undoStack->push(cmd);
   });

   createToolBars();
}

MainWindow::~MainWindow()
{
   delete ui;
}

void MainWindow::createToolBars()
{
   m_selectionTool = new SelectionTool(m_scene);
   m_newLineDrawerTool = new NewLineDrawerTool(m_scene);
   m_scene->setTool(m_selectionTool);

   auto m_mainToolBar = new QToolBar("Main Toolbar", this);
   m_mainToolBar->setFloatable(false);
   m_mainToolBar->setMovable(true);

   m_toolActionGroup = new QActionGroup(this);
   m_toolActionGroup->setExclusive(true);

   auto mainEditorSelectAction = m_mainToolBar->addAction(m_selectionTool->nameToShow());
   auto newLineDrawerSelectAction = m_mainToolBar->addAction(m_newLineDrawerTool->nameToShow());

   mainEditorSelectAction->setCheckable(true);
   mainEditorSelectAction->setChecked(true);
   mainEditorSelectAction->setActionGroup(m_toolActionGroup);

   newLineDrawerSelectAction->setCheckable(true);
   newLineDrawerSelectAction->setActionGroup(m_toolActionGroup);

   connect(m_toolActionGroup, &QActionGroup::triggered, this, &MainWindow::onToolSelected);

   addToolBar(Qt::TopToolBarArea, m_mainToolBar);

   m_newLineDrawerTool->m_bendNumber = ui->spinBoxBendsNumber->value();
   m_newLineDrawerTool->m_isFirstPartHorizontal = ui->checkBox->isEnabled();
}

void MainWindow::onToolSelected(QAction* action)
{
   if (action->text() == m_selectionTool->nameToShow())
   {
      m_scene->setTool(m_selectionTool);
      action->setChecked(true);
   }
   else if (action->text() == m_newLineDrawerTool->nameToShow())
   {
      m_scene->setTool(m_newLineDrawerTool);
      action->setChecked(true);
   }
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
   m_undoStack->push(new ReplaceErdModelCommand(m_scene->erdModel(), erdModel, m_scene));
   m_scene->loadModel(erdModel);
}

void MainWindow::on_actionSave_as_triggered()
{
   auto pathToFile = QFileDialog::getSaveFileName(this, "Save", QDir::homePath(), "*.json");
   auto jsonDoc = QJsonDocument(ERDMapper::toJson(m_scene->erdModel()));
   saveTextFile(pathToFile, QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Indented)));
}


void MainWindow::on_checkBox_clicked(bool checked)
{
   m_newLineDrawerTool->m_isFirstPartHorizontal = checked;
}


void MainWindow::on_spinBoxBendsNumber_editingFinished()
{
   m_newLineDrawerTool->m_bendNumber = ui->spinBoxBendsNumber->value();
}

