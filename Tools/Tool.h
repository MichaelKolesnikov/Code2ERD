#pragma once
#include <QObject>

class QGraphicsSceneMouseEvent;
class QGraphicsSceneContextMenuEvent;
class QGraphicsSceneWheelEvent;
class QKeyEvent;
class Scene;

class Tool : public QObject
{
   Q_OBJECT
public:
   explicit Tool(Scene*);
   virtual ~Tool() = default;

   virtual void activated() = 0;
   virtual void deactivated() = 0;
   virtual const char* nameToShow() const = 0;

protected:
   virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent*);
   virtual void keyPressEvent(QKeyEvent*);
   virtual void keyReleaseEvent(QKeyEvent*);
   virtual void mousePressEvent(QGraphicsSceneMouseEvent*);
   virtual void mouseMoveEvent(QGraphicsSceneMouseEvent*);
   virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
   virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent*);
   virtual void wheelEvent(QGraphicsSceneWheelEvent*);

   Scene* const m_scene;

   friend class Scene;
};
