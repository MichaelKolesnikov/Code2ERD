#include "Tool.h"
#include "Scene.h"

Tool::Tool(Scene *diagramScene)
   : QObject(diagramScene), m_scene(diagramScene)
{

}

// defaults
void Tool::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
   m_scene->baseContextMenuEvent(event);
}
void Tool::keyPressEvent(QKeyEvent *event)
{
   m_scene->baseKeyPressEvent(event);
}
void Tool::keyReleaseEvent(QKeyEvent *event)
{
   m_scene->baseKeyReleaseEvent(event);
}
void Tool::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
   m_scene->baseMousePressEvent(event);
}
void Tool::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
   m_scene->baseMouseMoveEvent(event);
}
void Tool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
   m_scene->baseMouseReleaseEvent(event);
}
void Tool::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
   m_scene->baseMouseDoubleClickEvent(event);
}
void Tool::wheelEvent(QGraphicsSceneWheelEvent *event)
{
   m_scene->baseWheelEvent(event);
}
