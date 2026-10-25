#pragma once
#include <QFont>
#include "Theme.h"

// What all the diagram items are drawn with; owned by the scene, the items keep a pointer to it
struct DiagramStyle
{
   Theme theme = Theme::light();
   bool showAttributes = true;
   bool snapToGrid = false;
   bool exporting = false; // no grid, no hover and selection marks

   QFont nodeFont;
   QFont entityFont; // bold
   QFont labelFont;  // cardinalities and roles
   qreal gridSize = 20;
};
