#pragma once
#include "ERDUndoCommand.h"

class ERDItemModel;
class ERDModel;

class AddRemoveCommand : public ERDUndoCommand
{
public:
   enum Operation {Add, Remove};

   AddRemoveCommand(ERDItemModel*, AddRemoveCommand::Operation, ERDModel*, QUndoCommand* parent = nullptr);
   ~AddRemoveCommand();

   virtual void undo();
   virtual void redo();

private:
   ERDItemModel* m_model;
   AddRemoveCommand::Operation m_operation;
   ERDModel* m_erdModel;
   bool m_isRemoved;
};

