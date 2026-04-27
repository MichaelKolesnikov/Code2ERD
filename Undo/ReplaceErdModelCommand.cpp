#include "ReplaceErdModelCommand.h"
#include "Models/ERDModel.h"
#include "Scene.h"

ReplaceErdModelCommand::ReplaceErdModelCommand(ERDModel* oldErdModel, ERDModel *newErdModel, Scene *erdScene, QUndoCommand *parent)
   : UndoCommand("ReplaceErdModel", parent), m_oldErdModel(oldErdModel), m_newErdModel(newErdModel), m_erdScene(erdScene), m_oldIsCur(true)
{

}

ReplaceErdModelCommand::~ReplaceErdModelCommand()
{
   if (m_oldIsCur)
   {
      delete m_newErdModel;
   }
}

void ReplaceErdModelCommand::undo()
{
   m_erdScene->loadModel(m_oldErdModel);
   m_oldIsCur = true;
}

void ReplaceErdModelCommand::redo()
{
   m_erdScene->loadModel(m_newErdModel);
   m_oldIsCur = false;
}

bool ReplaceErdModelCommand::hasEffect() const
{
   return m_oldErdModel != m_newErdModel;
}
