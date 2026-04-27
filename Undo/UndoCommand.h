#pragma once
#include <QUndoCommand>

class UndoCommand : public QUndoCommand
{
public:
   UndoCommand(const QString& text, QUndoCommand* parent = nullptr);
   virtual ~UndoCommand() = default;

   virtual bool hasEffect() const = 0;
};

