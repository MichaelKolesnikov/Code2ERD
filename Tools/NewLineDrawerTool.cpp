#include "NewLineDrawerTool.h"
#include "Scene.h"
#include "Models/LineModel.h"
#include "Items/LineItem.h"
#include "Undo/AddRemoveCommand.h"
#include <QGraphicsSceneMouseEvent>
#include "Managers/LineGeometryManager.h"

NewLineDrawerTool::NewLineDrawerTool(Scene *scene)
   : Tool(scene)
   , m_lineModel(new LineModel())
   , m_lineItem(new LineItem(m_lineModel))
{
}

NewLineDrawerTool::~NewLineDrawerTool()
{
}

void NewLineDrawerTool::activated()
{
}

void NewLineDrawerTool::deactivated()
{
}

const char* NewLineDrawerTool::nameToShow() const
{
   return "New Line Drawer";
}

void NewLineDrawerTool::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
   if (event->buttons() & Qt::LeftButton)
   {
      m_initialMouseScenePos = event->scenePos();
      m_lineModel->setNodes({m_initialMouseScenePos});
      m_scene->addItem(m_lineItem);
   }
}

void NewLineDrawerTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
   LineGeometryManager::set(m_lineModel, m_initialMouseScenePos, event->scenePos(), m_isFirstPartHorizontal, m_bendNumber);
}

void NewLineDrawerTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
   mouseMoveEvent(event);
   m_scene->removeItem(m_lineItem);
   if (LineGeometryManager::isVeryShortToExist(m_lineModel))
   {
      return;
   }
   m_scene->pushCommand(new AddRemoveCommand(new LineModel(m_lineModel->dto()), AddRemoveCommand::Add, m_scene->erdModel()));
}
