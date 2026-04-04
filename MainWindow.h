#pragma once
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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

private:
   Ui::MainWindow *ui;

   ERDScene* m_erdScene;
   QUndoStack* m_undoStack;
};
