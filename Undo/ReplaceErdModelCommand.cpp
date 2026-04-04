#include "ReplaceErdModelCommand.h"
#include "Models/ERDModel.h"
#include "ERDScene.h"

ReplaceErdModelCommand::ReplaceErdModelCommand(ERDModel* oldErdModel, ERDModel *newErdModel, ERDScene *erdScene, QUndoCommand *parent)
   : ERDUndoCommand("ReplaceErdModel", parent), m_oldErdModel(oldErdModel), m_newErdModel(newErdModel), m_erdScene(erdScene), m_oldIsCur(true)
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
