#include "ERDSceneView.h"
#include <QMouseEvent>
#include <QScrollBar>

void ERDSceneView::init()
{
   m_isPanning = false;
   setOptimizationFlags(IndirectPainting);
   setViewportUpdateMode(SmartViewportUpdate);
   setResizeAnchor(AnchorUnderMouse);
   setTransformationAnchor(AnchorUnderMouse);
}

void ERDSceneView::mousePressEvent(QMouseEvent *event)
{
   if ((event->buttons() & Qt::LeftButton) && (event->modifiers() & Qt::ShiftModifier))
   {
      m_isPanning = true;
      m_startPanningPosition = event->pos();
      setCursor(Qt::ClosedHandCursor);
      event->accept();
   }
   else
   {
      QGraphicsView::mousePressEvent(event);
   }
}

void ERDSceneView::mouseMoveEvent(QMouseEvent *event)
{
   if (m_isPanning)
   {
      auto curPanningPosition = event->pos();
      auto delta = curPanningPosition - m_startPanningPosition;
      horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
      verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
      m_startPanningPosition = event->pos();
      event->accept();
   }
   else
   {
      QGraphicsView::mouseMoveEvent(event);
   }
}

void ERDSceneView::mouseReleaseEvent(QMouseEvent *event)
{
   if (m_isPanning)
   {
      unsetCursor();
      m_isPanning = false;
      event->accept();
   }
   else
   {
      QGraphicsView::mouseReleaseEvent(event);
   }
}

void ERDSceneView::wheelEvent(QWheelEvent *event)
{
   if (event->modifiers() & Qt::ControlModifier)
   {
      qreal scaleFactor = 1.15;
      auto angleDelta = event->angleDelta();
      if (angleDelta.y() > 0)
      {
         scale(scaleFactor, scaleFactor);
      }
      else
      {
         scale(1.0 / scaleFactor, 1.0 / scaleFactor);
      }
      event->accept();
   }
   else
   {
      QGraphicsView::wheelEvent(event);
   }
}
