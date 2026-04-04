#pragma once
#include "ERDUndoCommand.h"

class ERDModel;
class ERDScene;

class ReplaceErdModelCommand : public ERDUndoCommand
{
public:
   ReplaceErdModelCommand(ERDModel* m_oldErdModel, ERDModel* newErdModel, ERDScene* erdScene, QUndoCommand* parent = nullptr);
   ~ReplaceErdModelCommand();

   virtual void undo() override;
   virtual void redo() override;

private:
   ERDModel* m_oldErdModel;
   ERDModel* m_newErdModel;
   ERDScene* m_erdScene;
   bool m_oldIsCur;
};

