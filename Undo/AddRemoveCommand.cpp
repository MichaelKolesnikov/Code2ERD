#include "AddRemoveCommand.h"
#include "Models/ERDModel.h"

AddRemoveCommand::AddRemoveCommand(ERDItemModel* model, Operation operation, ERDModel* erdModel, QUndoCommand* parent)
   : ERDUndoCommand("AddRemoveCommand", parent), m_model(model), m_operation(operation), m_erdModel(erdModel)
{
}

void AddRemoveCommand::undo()
{
   if (m_operation == Add)
   {
      m_erdModel->remove(m_model);
   }
   else
   {
      m_erdModel->add(m_model);
   }
}

void AddRemoveCommand::redo()
{
   if (m_operation == Add)
   {
      m_erdModel->add(m_model);
   }
   else
   {
      m_erdModel->remove(m_model);
   }
}
