#pragma once
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ERDModel;
class ERDScene;
class QUndoStack;

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   MainWindow(QWidget *parent = nullptr);
   ~MainWindow();

   void keyPressEvent(QKeyEvent *event) override;

private slots:
   void on_actionOpen_triggered();

   void on_actionSave_as_triggered();

private:
   Ui::MainWindow *ui;

   ERDScene* m_erdScene;
   QUndoStack* m_undoStack;
};
