#include "ERDUndoCommand.h"

ERDUndoCommand::ERDUndoCommand(const QString &text, QUndoCommand *parent) : QUndoCommand(parent)
{
   setText(text);
}
