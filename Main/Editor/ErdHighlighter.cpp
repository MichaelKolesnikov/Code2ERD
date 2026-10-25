#include "ErdHighlighter.h"
#include <QRegularExpression>

ErdHighlighter::ErdHighlighter(QTextDocument *parent)
   : QSyntaxHighlighter(parent)
{
   setTheme(Theme::light());
}

void ErdHighlighter::setTheme(const Theme &theme)
{
   m_keyword.setForeground(theme.keyword);
   m_keyword.setFontWeight(QFont::Bold);
   m_setting.setForeground(theme.setting);
   m_cardinality.setForeground(theme.cardinality);
   m_cardinality.setFontWeight(QFont::DemiBold);
   m_string.setForeground(theme.string);
   m_comment.setForeground(theme.comment);
   m_comment.setFontItalic(true);
   m_number.setForeground(theme.number);
   m_symbol.setForeground(theme.symbol);
   m_symbol.setFontWeight(QFont::Bold);
   m_quotedName.setForeground(theme.editorText);
   m_quotedName.setFontWeight(QFont::DemiBold);
   rehighlight();
}

void ErdHighlighter::highlightBlock(const QString &text)
{
   // strings and comments first: nothing else is highlighted inside them
   QVector<bool> taken(text.size(), false);
   auto mark = [&](int start, int length, const QTextCharFormat& format) {
      setFormat(start, length, format);
      for (int i = start; i < start + length && i < taken.size(); ++i)
      {
         taken[i] = true;
      }
   };

   int pos = 0;
   setCurrentBlockState(Normal);
   if (previousBlockState() == InMultiLineString)
   {
      const int end = text.indexOf(QLatin1String("'''"));
      if (end < 0)
      {
         mark(0, text.size(), m_string);
         setCurrentBlockState(InMultiLineString);
         return;
      }
      mark(0, end + 3, m_string);
      pos = end + 3;
   }
   else if (previousBlockState() == InBlockComment)
   {
      const int end = text.indexOf(QLatin1String("*/"));
      if (end < 0)
      {
         mark(0, text.size(), m_comment);
         setCurrentBlockState(InBlockComment);
         return;
      }
      mark(0, end + 2, m_comment);
      pos = end + 2;
   }

   while (pos < text.size())
   {
      const QChar c = text[pos];
      if (c == '/' && pos + 1 < text.size() && text[pos + 1] == '/')
      {
         mark(pos, text.size() - pos, m_comment);
         break;
      }
      if (c == '/' && pos + 1 < text.size() && text[pos + 1] == '*')
      {
         const int end = text.indexOf(QLatin1String("*/"), pos + 2);
         if (end < 0)
         {
            mark(pos, text.size() - pos, m_comment);
            setCurrentBlockState(InBlockComment);
            break;
         }
         mark(pos, end + 2 - pos, m_comment);
         pos = end + 2;
         continue;
      }
      if (text.midRef(pos, 3) == QLatin1String("'''"))
      {
         const int end = text.indexOf(QLatin1String("'''"), pos + 3);
         if (end < 0)
         {
            mark(pos, text.size() - pos, m_string);
            setCurrentBlockState(InMultiLineString);
            break;
         }
         mark(pos, end + 3 - pos, m_string);
         pos = end + 3;
         continue;
      }
      if (c == '\'' || c == '"')
      {
         int end = pos + 1;
         while (end < text.size() && text[end] != c)
         {
            end += text[end] == '\\' ? 2 : 1;
         }
         const int length = std::min(end + 1, text.size()) - pos;
         mark(pos, length, c == '\'' ? m_string : m_quotedName);
         pos += length;
         continue;
      }
      ++pos;
   }

   auto apply = [&](const QRegularExpression& expression, const QTextCharFormat& format, int group = 0) {
      auto it = expression.globalMatch(text);
      while (it.hasNext())
      {
         const QRegularExpressionMatch match = it.next();
         const int start = match.capturedStart(group);
         if (start >= 0 && start < taken.size() && !taken[start])
         {
            setFormat(start, match.capturedLength(group), format);
         }
      }
   };

   static const QRegularExpression keywords(QStringLiteral("^\\s*(Entity|Relationship)\\b"), QRegularExpression::CaseInsensitiveOption);
   static const QRegularExpression colors(QStringLiteral("#[0-9A-Fa-f]{3,6}\\b"));
   static const QRegularExpression braces(QStringLiteral("[{}]"));
   // the cardinality of an entity in a relationship: (1), (0, N), (1, *)
   static const QRegularExpression cardinalities(QStringLiteral("\\(\\s*[\\w*]+\\s*(,\\s*[\\w*]+\\s*)?\\)"));
   static const QRegularExpression settingsBlock(QStringLiteral("\\[[^\\]]*\\]"));
   static const QRegularExpression settings(
         QStringLiteral("\\b(weak|identifying|key|pk|partial key|partial|multivalued|multi|derived|total|role|color|note)\\b"),
         QRegularExpression::CaseInsensitiveOption);

   apply(keywords, m_keyword, 1);
   apply(cardinalities, m_cardinality);
   apply(colors, m_number);
   apply(braces, m_symbol);

   auto blocks = settingsBlock.globalMatch(text);
   while (blocks.hasNext())
   {
      const QRegularExpressionMatch block = blocks.next();
      auto it = settings.globalMatch(block.captured());
      while (it.hasNext())
      {
         const QRegularExpressionMatch match = it.next();
         const int start = block.capturedStart() + match.capturedStart();
         if (!taken.value(start))
         {
            setFormat(start, match.capturedLength(), m_setting);
         }
      }
   }
}
