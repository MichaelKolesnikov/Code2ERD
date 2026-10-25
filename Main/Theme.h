#pragma once
#include "Main_global.h"
#include <QColor>
#include <QPalette>

// Colors of the diagram, the editor and the window, for the light and the dark look
struct MAIN_EXPORT Theme
{
   bool dark = false;

   // diagram
   QColor canvas;
   QColor gridDot;
   QColor entityFill;
   QColor entityBorder;
   QColor relationshipFill;
   QColor relationshipBorder;
   QColor attributeFill;
   QColor attributeBorder;
   QColor nodeText;
   QColor line;
   QColor label;
   QColor highlight;
   QColor selection;

   // editor
   QColor editorBackground;
   QColor editorText;
   QColor gutterBackground;
   QColor gutterText;
   QColor currentLine;
   QColor keyword;
   QColor setting;
   QColor cardinality;
   QColor string;
   QColor comment;
   QColor number;
   QColor symbol;
   QColor error;
   QColor findMatch;
   QColor bannerBackground;
   QColor bannerText;

   static Theme light();
   static Theme darkTheme();

   QPalette palette() const;
   // Readable text over a color: black on light ones, white on dark ones
   static QColor textOn(const QColor& background);
};
