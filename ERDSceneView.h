#pragma once
#include <QGraphicsView>

class ERDSceneView : public QGraphicsView
{
   Q_OBJECT
public:
   using QGraphicsView::QGraphicsView;

   void init();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    bool m_isPanning;
    QPointF m_startPanningPosition;
};

