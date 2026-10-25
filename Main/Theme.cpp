#include "Theme.h"

Theme Theme::light()
{
   Theme theme;
   theme.dark = false;

   theme.canvas = QColor("#f4f6f8");
   theme.gridDot = QColor("#d5dae0");
   theme.entityFill = QColor("#dcebfa");
   theme.entityBorder = QColor("#2f5d8a");
   theme.relationshipFill = QColor("#fde6d2");
   theme.relationshipBorder = QColor("#b0602a");
   theme.attributeFill = QColor("#ffffff");
   theme.attributeBorder = QColor("#6b7785");
   theme.nodeText = QColor("#1f2933");
   theme.line = QColor("#5f6b7a");
   theme.label = QColor("#9b2c2c");
   theme.highlight = QColor("#1e88e5");
   theme.selection = QColor("#1e88e5");

   theme.editorBackground = QColor("#ffffff");
   theme.editorText = QColor("#24292e");
   theme.gutterBackground = QColor("#f6f8fa");
   theme.gutterText = QColor("#959da5");
   theme.currentLine = QColor("#f1f8ff");
   theme.keyword = QColor("#d73a49");
   theme.setting = QColor("#6f42c1");
   theme.cardinality = QColor("#e36209");
   theme.string = QColor("#22863a");
   theme.comment = QColor("#6a737d");
   theme.number = QColor("#005cc5");
   theme.symbol = QColor("#d73a49");
   theme.error = QColor("#e53935");
   theme.findMatch = QColor("#ffe58f");
   theme.bannerBackground = QColor("#fdecea");
   theme.bannerText = QColor("#b71c1c");
   return theme;
}

Theme Theme::darkTheme()
{
   Theme theme;
   theme.dark = true;

   theme.canvas = QColor("#1b1f24");
   theme.gridDot = QColor("#2e353d");
   theme.entityFill = QColor("#1f3a56");
   theme.entityBorder = QColor("#6fa8dc");
   theme.relationshipFill = QColor("#4a3020");
   theme.relationshipBorder = QColor("#e59866");
   theme.attributeFill = QColor("#262c33");
   theme.attributeBorder = QColor("#8b98a5");
   theme.nodeText = QColor("#e1e6eb");
   theme.line = QColor("#8492a6");
   theme.label = QColor("#ff9e80");
   theme.highlight = QColor("#4fa3ff");
   theme.selection = QColor("#4fa3ff");

   theme.editorBackground = QColor("#1e2228");
   theme.editorText = QColor("#d7dce2");
   theme.gutterBackground = QColor("#1a1d22");
   theme.gutterText = QColor("#5c6773");
   theme.currentLine = QColor("#262b33");
   theme.keyword = QColor("#ff7b72");
   theme.setting = QColor("#d2a8ff");
   theme.cardinality = QColor("#ffa657");
   theme.string = QColor("#a5d6a7");
   theme.comment = QColor("#8b949e");
   theme.number = QColor("#79c0ff");
   theme.symbol = QColor("#ff7b72");
   theme.error = QColor("#ff6b6b");
   theme.findMatch = QColor("#6e5a12");
   theme.bannerBackground = QColor("#4a1f1f");
   theme.bannerText = QColor("#ffb4ab");
   return theme;
}

QPalette Theme::palette() const
{
   if (!dark)
   {
      QPalette palette;
      palette.setColor(QPalette::Window, QColor("#eef1f4"));
      palette.setColor(QPalette::WindowText, QColor("#1f2933"));
      palette.setColor(QPalette::Base, QColor("#ffffff"));
      palette.setColor(QPalette::AlternateBase, QColor("#f6f8fa"));
      palette.setColor(QPalette::Text, QColor("#1f2933"));
      palette.setColor(QPalette::Button, QColor("#f4f6f8"));
      palette.setColor(QPalette::ButtonText, QColor("#1f2933"));
      palette.setColor(QPalette::ToolTipBase, QColor("#ffffff"));
      palette.setColor(QPalette::ToolTipText, QColor("#1f2933"));
      palette.setColor(QPalette::Highlight, QColor("#1e88e5"));
      palette.setColor(QPalette::HighlightedText, Qt::white);
      palette.setColor(QPalette::PlaceholderText, QColor("#9aa5b1"));
      palette.setColor(QPalette::Disabled, QPalette::Text, QColor("#9aa5b1"));
      palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor("#9aa5b1"));
      palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor("#9aa5b1"));
      return palette;
   }
   QPalette palette;
   palette.setColor(QPalette::Window, QColor("#22272e"));
   palette.setColor(QPalette::WindowText, QColor("#d7dce2"));
   palette.setColor(QPalette::Base, QColor("#1e2228"));
   palette.setColor(QPalette::AlternateBase, QColor("#262c33"));
   palette.setColor(QPalette::Text, QColor("#d7dce2"));
   palette.setColor(QPalette::Button, QColor("#2d333b"));
   palette.setColor(QPalette::ButtonText, QColor("#d7dce2"));
   palette.setColor(QPalette::ToolTipBase, QColor("#2d333b"));
   palette.setColor(QPalette::ToolTipText, QColor("#d7dce2"));
   palette.setColor(QPalette::Highlight, QColor("#2f81f7"));
   palette.setColor(QPalette::HighlightedText, Qt::white);
   palette.setColor(QPalette::Light, QColor("#3a424c"));
   palette.setColor(QPalette::Midlight, QColor("#323941"));
   palette.setColor(QPalette::Mid, QColor("#2a3037"));
   palette.setColor(QPalette::Dark, QColor("#15181c"));
   palette.setColor(QPalette::Shadow, QColor("#0b0d10"));
   palette.setColor(QPalette::Link, QColor("#4fa3ff"));
   palette.setColor(QPalette::PlaceholderText, QColor("#6b7785"));
   palette.setColor(QPalette::Disabled, QPalette::Text, QColor("#6b7785"));
   palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor("#6b7785"));
   palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor("#6b7785"));
   return palette;
}

QColor Theme::textOn(const QColor &background)
{
   const double luminance = 0.299 * background.redF() + 0.587 * background.greenF() + 0.114 * background.blueF();
   return luminance > 0.62 ? QColor("#1f2933") : QColor(Qt::white);
}
