#pragma once
#include <QUndoCommand>

class UndoCommand : public QUndoCommand
{
public:
   UndoCommand(const QString& text, QUndoCommand* parent = nullptr);
   virtual ~UndoCommand() = default;
};

