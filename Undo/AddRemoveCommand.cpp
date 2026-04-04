#include "AddRemoveCommand.h"
#include "Models/ERDModel.h"

AddRemoveCommand::AddRemoveCommand(ERDItemModel* model, Operation operation, ERDModel* erdModel, QUndoCommand* parent)
   : ERDUndoCommand("AddRemoveCommand", parent), m_model(model), m_operation(operation), m_erdModel(erdModel), m_isRemoved(operation == Add ? true : false)
{
}

AddRemoveCommand::~AddRemoveCommand()
{
   if (m_isRemoved)
   {
      delete m_model;
   }
}

void AddRemoveCommand::undo()
{
   if (m_operation == Add)
   {
      m_erdModel->remove(m_model);
      m_isRemoved = true;
   }
   else
   {
      m_erdModel->add(m_model);
      m_isRemoved = false;
   }
}

void AddRemoveCommand::redo()
{
   if (m_operation == Add)
   {
      m_erdModel->add(m_model);
      m_isRemoved = false;
   }
   else
   {
      m_erdModel->remove(m_model);
      m_isRemoved = true;
   }
}
