#include "ChangeLineNodesCommand.h"
#include "Models/LineModel.h"


ChangeLineNodesCommand::ChangeLineNodesCommand(LineModel *model, const QVector<QPointF> &nodes, QUndoCommand *parent)
   : UndoCommand("Change line nodes", parent)
   , m_model(model)
   , m_old(model->nodes())
   , m_new(nodes)
{

}

void ChangeLineNodesCommand::redo()
{
   m_model->setNodes(m_new);
}

void ChangeLineNodesCommand::undo()
{
   m_model->setNodes(m_old);
}

bool ChangeLineNodesCommand::hasEffect() const
{
   return m_old != m_new;
}
