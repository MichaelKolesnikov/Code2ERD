#pragma once
#include "Core_global.h"
#include <QVector>
#include "DTO/DiagramDTO.h"
#include "ErdError.h"

struct ErdParseResult
{
   DiagramDTO diagram;
   QVector<ErdError> errors; // sorted by position

   bool ok() const
   {
      return errors.isEmpty();
   }
};

namespace ErdParser
{
   // Parses as much as possible: a broken element or line is reported and skipped, the rest is still parsed.
   // The participants of the relationships are checked after parsing, so the entities may be written in any order
   CORE_EXPORT ErdParseResult parse(const QString& source);
}
