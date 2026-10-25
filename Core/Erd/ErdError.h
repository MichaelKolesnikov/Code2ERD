#pragma once
#include <QString>

struct ErdError
{
   int line = 0;   // 1-based
   int column = 0; // 1-based
   int length = 1; // characters to underline
   QString message;
};
