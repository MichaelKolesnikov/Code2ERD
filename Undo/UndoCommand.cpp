#include "UndoCommand.h"

UndoCommand::UndoCommand(const QString &text, QUndoCommand *parent) : QUndoCommand(parent)
{
   setText(text);
}
