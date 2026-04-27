#pragma once
#include "UndoCommand.h"

class ERDModel;
class Scene;

class ReplaceErdModelCommand : public UndoCommand
{
public:
   ReplaceErdModelCommand(ERDModel* m_oldErdModel, ERDModel* newErdModel, Scene* erdScene, QUndoCommand* parent = nullptr);
   ~ReplaceErdModelCommand();

   virtual void undo() override;
   virtual void redo() override;

   virtual bool hasEffect() const override;

private:
   ERDModel* m_oldErdModel;
   ERDModel* m_newErdModel;
   Scene* m_erdScene;
   bool m_oldIsCur;
};

