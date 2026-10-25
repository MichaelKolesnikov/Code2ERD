#pragma once
#include "Main_global.h"
#include <QPlainTextEdit>
#include "PIMPL.h"
#include "Erd/ErdError.h"
struct Theme;

// The ERD text: line numbers, syntax highlighting, parse errors under the text and in the gutter,
// auto-indentation and closing brackets
class MAIN_EXPORT ErdEditor : public QPlainTextEdit
{
   Q_OBJECT
public:
   explicit ErdEditor(QWidget* parent = nullptr);
   ~ErdEditor() override;

   void setErrors(const QVector<ErdError>&);
   void setTheme(const Theme&);
   void setEditorFont(const QFont&);
   void zoomText(int steps);

   // 1-based
   int currentLine() const;
   void goToLine(int line);

   void toggleComment();
   // Highlights all occurrences, returns their count
   int setSearchHighlight(const QString& text, bool caseSensitive);

   // used by the line number area
   int gutterWidth() const;
   void paintGutter(QPaintEvent*);
   void showErrorToolTip(const QPoint& viewportPos, const QPoint& globalPos);

signals:
   void fontSizeChanged(qreal pointSize);

protected:
   bool viewportEvent(QEvent*) override;
   void resizeEvent(QResizeEvent*) override;
   void keyPressEvent(QKeyEvent*) override;
   void wheelEvent(QWheelEvent*) override;

PIMPL
};
