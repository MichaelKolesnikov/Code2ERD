#pragma once
#include <QUndoCommand>

class ERDUndoCommand : public QUndoCommand
{
public:
   ERDUndoCommand(const QString& text, QUndoCommand* parent = nullptr);
   virtual ~ERDUndoCommand() = default;
};

