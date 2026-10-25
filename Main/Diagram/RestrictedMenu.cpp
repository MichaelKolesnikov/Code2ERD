#include "RestrictedMenu.h"
#include <QMouseEvent>

RestrictedMenu::RestrictedMenu(QWidget *parent) : QMenu(parent) {}

void RestrictedMenu::mousePressEvent(QMouseEvent *event)
{
   if (event->button() != Qt::LeftButton)
   {
      event->ignore();
      return;
   }
   QMenu::mousePressEvent(event);
}

void RestrictedMenu::mouseReleaseEvent(QMouseEvent *event)
{
   if (event->button() != Qt::LeftButton)
   {
      event->ignore();
      return;
   }
   QMenu::mouseReleaseEvent(event);
}
