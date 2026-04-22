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
   m_scene->addItem(m_lineItem);
   m_lineItem->hide();
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
      m_lineItem->show();
   }
}

void NewLineDrawerTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
   if (event->buttons() & Qt::LeftButton)
   {
      LineGeometryManager::set(m_lineModel, m_initialMouseScenePos, event->scenePos(), m_isFirstPartHorizontal, m_bendNumber);
   }
}

void NewLineDrawerTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
   if (event->button() == Qt::LeftButton)
   {
      mouseMoveEvent(event);
      if (LineGeometryManager::isVeryShortToExist(m_lineModel))
      {
         return;
      }
      auto dto = m_lineModel->dto();
      dto.id = QUuid::createUuid().toString();
      m_scene->pushCommand(new AddRemoveCommand(new LineModel(dto), AddRemoveCommand::Add, m_scene->erdModel()));
   }
   m_lineItem->hide();
}
