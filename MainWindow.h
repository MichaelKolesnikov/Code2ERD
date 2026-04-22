#pragma once
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ERDModel;
class Scene;
class QUndoStack;
class SelectionTool;
class NewLineDrawerTool;
class QActionGroup;

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   MainWindow(QWidget *parent = nullptr);
   ~MainWindow();

   void keyPressEvent(QKeyEvent *event) override;

   void createToolBars();

public slots:
   void onToolSelected(QAction *action);

private slots:
   void on_actionOpen_triggered();

   void on_actionSave_as_triggered();

   void on_checkBox_clicked(bool checked);

   void on_spinBoxBendsNumber_editingFinished();

private:
   Ui::MainWindow *ui;

   Scene* m_scene;
   QUndoStack* m_undoStack;

   QActionGroup *m_toolActionGroup;
   SelectionTool* m_selectionTool;
   NewLineDrawerTool* m_newLineDrawerTool;
};
