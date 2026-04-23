#pragma once
#include "UndoCommand.h"
class LineModel;

class ChangeLineNodesCommand : public UndoCommand
{
public:
   ChangeLineNodesCommand(LineModel*, const QVector<QPointF>&, QUndoCommand* parent = nullptr);

   void redo() override;
   void undo() override;

   virtual bool hasEffect() const override;

private:
   LineModel* m_model;
   QVector<QPointF> m_old, m_new;
};
