#include "NodeItem.h"
#include "DiagramScene.h"
#include "DiagramStyle.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QFontMetricsF>
#include <cmath>

namespace
{
   const qreal BoundingMargin = 4;
   const qreal DoubleBorderGap = 5;
   const qreal BorderWidth = 1.6;
   const qreal HighlightWidth = 2.6;

   QString escapeHtml(const QString& text)
   {
      return text.toHtmlEscaped().replace('\n', QStringLiteral("<br>"));
   }
}

NodeItem::NodeItem(const NodeInfo &info, const DiagramStyle *style, NodeItem *owner)
   : m_info(info), m_style(style), m_owner(owner)
{
   setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges);
   setAcceptHoverEvents(true);
   setZValue(info.kind == NodeInfo::Kind::Attribute ? 9 : 10);
   setToolTip(tooltip());
   relayout();
}

int NodeItem::type() const
{
   return Type;
}

const NodeInfo& NodeItem::info() const
{
   return m_info;
}

NodeItem* NodeItem::owner() const
{
   return m_owner;
}

void NodeItem::relayout()
{
   prepareGeometryChange();
   const QFontMetricsF metrics(m_info.kind == NodeInfo::Kind::Entity ? m_style->entityFont : m_style->nodeFont);
   const qreal text = metrics.horizontalAdvance(m_info.name);
   const qreal height = metrics.height();
   switch (m_info.kind)
   {
   case NodeInfo::Kind::Entity:
      m_size = QSizeF(std::max(110.0, text + 40), std::max(46.0, height + 24));
      break;
   case NodeInfo::Kind::Relationship:
      // the text fits into the middle of the diamond
      m_size = QSizeF(std::max(130.0, text * 1.6 + 50), std::max(66.0, height * 2.4 + 18));
      break;
   case NodeInfo::Kind::Attribute:
      m_size = QSizeF(std::max(70.0, text + 38), std::max(34.0, height + 16));
      break;
   }
   m_size = QSizeF(std::ceil(m_size.width()), std::ceil(m_size.height()));
   update();
}

QSizeF NodeItem::size() const
{
   return m_size;
}

QPointF NodeItem::boundaryPoint(const QPointF &towards) const
{
   const QPointF d = mapFromScene(towards);
   const qreal a = m_size.width() / 2;
   const qreal b = m_size.height() / 2;
   if (std::abs(d.x()) < 1e-6 && std::abs(d.y()) < 1e-6)
   {
      return scenePos();
   }
   qreal t = 1;
   switch (m_info.kind)
   {
   case NodeInfo::Kind::Entity:
      t = std::min(std::abs(d.x()) > 1e-6 ? a / std::abs(d.x()) : 1e9, std::abs(d.y()) > 1e-6 ? b / std::abs(d.y()) : 1e9);
      break;
   case NodeInfo::Kind::Relationship:
      t = 1 / (std::abs(d.x()) / a + std::abs(d.y()) / b);
      break;
   case NodeInfo::Kind::Attribute:
      t = 1 / std::hypot(d.x() / a, d.y() / b);
      break;
   }
   return mapToScene(d * t);
}

void NodeItem::setHighlighted(bool highlighted)
{
   if (m_highlighted != highlighted)
   {
      m_highlighted = highlighted;
      update();
   }
}

QRectF NodeItem::boundingRect() const
{
   return QRectF(-m_size.width() / 2, -m_size.height() / 2, m_size.width(), m_size.height())
         .adjusted(-BoundingMargin, -BoundingMargin, BoundingMargin, BoundingMargin);
}

QPainterPath NodeItem::outline(qreal inset) const
{
   const QRectF rect = QRectF(-m_size.width() / 2, -m_size.height() / 2, m_size.width(), m_size.height());
   QPainterPath path;
   switch (m_info.kind)
   {
   case NodeInfo::Kind::Entity:
      path.addRect(rect.adjusted(inset, inset, -inset, -inset));
      break;
   case NodeInfo::Kind::Relationship:
   {
      // the inner diamond is parallel to the outer one: its corners move by more than the inset
      const qreal a = rect.width() / 2;
      const qreal b = rect.height() / 2;
      const qreal edge = std::hypot(a, b);
      const qreal dx = inset * edge / b;
      const qreal dy = inset * edge / a;
      path.moveTo(-a + dx, 0);
      path.lineTo(0, -b + dy);
      path.lineTo(a - dx, 0);
      path.lineTo(0, b - dy);
      path.closeSubpath();
      break;
   }
   case NodeInfo::Kind::Attribute:
      path.addEllipse(rect.adjusted(inset, inset, -inset, -inset));
      break;
   }
   return path;
}

QPainterPath NodeItem::shape() const
{
   return outline(0);
}

QColor NodeItem::fillColor() const
{
   if (m_info.color.isValid())
   {
      return m_info.color;
   }
   switch (m_info.kind)
   {
   case NodeInfo::Kind::Entity:
      return m_style->theme.entityFill;
   case NodeInfo::Kind::Relationship:
      return m_style->theme.relationshipFill;
   case NodeInfo::Kind::Attribute:
      break;
   }
   return m_style->theme.attributeFill;
}

QColor NodeItem::borderColor() const
{
   if (m_info.color.isValid())
   {
      return m_style->theme.dark ? m_info.color.lighter(150) : m_info.color.darker(160);
   }
   switch (m_info.kind)
   {
   case NodeInfo::Kind::Entity:
      return m_style->theme.entityBorder;
   case NodeInfo::Kind::Relationship:
      return m_style->theme.relationshipBorder;
   case NodeInfo::Kind::Attribute:
      break;
   }
   return m_style->theme.attributeBorder;
}

void NodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
   const Theme& theme = m_style->theme;
   painter->setRenderHint(QPainter::Antialiasing);

   const bool marked = !m_style->exporting && (m_highlighted || isSelected());
   QPen pen(marked ? (isSelected() ? theme.selection : theme.highlight) : borderColor(), marked ? HighlightWidth : BorderWidth);
   pen.setJoinStyle(Qt::MiterJoin);
   if (m_info.dashedBorder)
   {
      pen.setStyle(Qt::DashLine);
   }
   painter->setPen(pen);
   painter->setBrush(fillColor());
   painter->drawPath(outline(0));
   if (m_info.doubleBorder)
   {
      painter->setBrush(Qt::NoBrush);
      painter->drawPath(outline(DoubleBorderGap));
   }

   const QFont& font = m_info.kind == NodeInfo::Kind::Entity ? m_style->entityFont : m_style->nodeFont;
   const QColor textColor = m_info.color.isValid() ? Theme::textOn(m_info.color) : theme.nodeText;
   painter->setFont(font);
   painter->setPen(textColor);
   const QRectF textRect = boundingRect();
   painter->drawText(textRect, Qt::AlignCenter, m_info.name);

   if (m_info.key || m_info.partialKey)
   {
      // underlined: a key fully, a partial key with dashes
      const QFontMetricsF metrics(font);
      const qreal width = metrics.horizontalAdvance(m_info.name);
      const qreal y = metrics.height() / 2 - metrics.descent() + 2;
      QPen underline(textColor, 1.2);
      if (m_info.partialKey)
      {
         underline.setStyle(Qt::DashLine);
      }
      painter->setPen(underline);
      painter->drawLine(QPointF(-width / 2, y), QPointF(width / 2, y));
   }
}

QVariant NodeItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
   if (change == ItemPositionChange && m_style->snapToGrid && scene())
   {
      const QPointF point = value.toPointF();
      const qreal grid = m_style->gridSize;
      return QPointF(std::round(point.x() / grid) * grid, std::round(point.y() / grid) * grid);
   }
   if (change == ItemPositionHasChanged)
   {
      const QPointF delta = pos() - m_lastPos;
      m_lastPos = pos();
      if (scene())
      {
         m_moved = true;
         diagramScene()->nodeMoved(this, delta);
      }
   }
   if (change == ItemSelectedHasChanged)
   {
      setZValue((m_info.kind == NodeInfo::Kind::Attribute ? 9 : 10) + (value.toBool() ? 2 : 0));
   }
   return QGraphicsItem::itemChange(change, value);
}

void NodeItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
   diagramScene()->setHoveredNode(this);
   QGraphicsItem::hoverEnterEvent(event);
}

void NodeItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
   diagramScene()->setHoveredNode(nullptr);
   QGraphicsItem::hoverLeaveEvent(event);
}

void NodeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
   m_moved = false;
   QGraphicsItem::mousePressEvent(event);
}

void NodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
   QGraphicsItem::mouseReleaseEvent(event);
   if (event->button() != Qt::LeftButton)
   {
      return;
   }
   if (m_moved)
   {
      diagramScene()->moveFinished();
      return;
   }
   // a click without a drag shows the definition in the editor
   diagramScene()->activate(m_info.line);
}

DiagramScene* NodeItem::diagramScene() const
{
   return static_cast<DiagramScene*>(scene());
}

QString NodeItem::tooltip() const
{
   QStringList traits;
   switch (m_info.kind)
   {
   case NodeInfo::Kind::Entity:
      traits << (m_info.doubleBorder ? QStringLiteral("weak entity") : QStringLiteral("entity"));
      break;
   case NodeInfo::Kind::Relationship:
      traits << (m_info.doubleBorder ? QStringLiteral("identifying relationship") : QStringLiteral("relationship"));
      break;
   case NodeInfo::Kind::Attribute:
      traits << QStringLiteral("attribute");
      if (m_info.key)
      {
         traits << QStringLiteral("key");
      }
      if (m_info.partialKey)
      {
         traits << QStringLiteral("partial key");
      }
      if (m_info.doubleBorder)
      {
         traits << QStringLiteral("multivalued");
      }
      if (m_info.dashedBorder)
      {
         traits << QStringLiteral("derived");
      }
      break;
   }
   QString text = QStringLiteral("<b>%1</b><br><i>%2</i>").arg(escapeHtml(m_info.name), traits.join(QStringLiteral(", ")));
   if (!m_info.note.isEmpty())
   {
      text += QStringLiteral("<br>") + escapeHtml(m_info.note);
   }
   return text;
}
