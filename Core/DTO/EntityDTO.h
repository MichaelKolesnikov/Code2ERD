#pragma once
#include <QVector>
#include "AttributeDTO.h"

struct EntityDTO
{
   QString name;
   bool weak = false; // double rectangle
   QString color;     // #rrggbb or empty
   QString note;
   QVector<AttributeDTO> attributes;
   int line = 0;    // first line of the definition
   int endLine = 0; // line of the closing brace, the first line without a body
};
