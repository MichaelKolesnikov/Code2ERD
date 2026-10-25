#include "EdgeItem.h"
#include "NodeItem.h"
#include "DiagramScene.h"
#include "DiagramStyle.h"
#include <QPainter>
#include <QPainterPathStroker>
#include <QGraphicsSceneMouseEvent>
#include <QFontMetricsF>
#include <QtMath>
#include <cmath>

namespace
{
   const qreal ParallelSpacing = 46;
   const qreal LabelDistance = 26; // from the entity along the line
   const qreal LabelOffset = 12;   // to the side of the line
   const qreal LineWidth = 1.4;
   const qreal TotalOuterWidth = 5;
   const qreal TotalInnerWidth = 1.8;
}

EdgeItem::EdgeItem(NodeItem *from, NodeItem *to, const Info &info, const DiagramStyle *style)
   : m_from(from), m_to(to), m_info(info), m_style(style)
{
   setAcceptHoverEvents(true);
   setAcceptedMouseButtons(Qt::LeftButton);
   setZValue(1);
   if (!info.role.isEmpty())
   {
      setToolTip(info.role);
   }
}

int EdgeItem::type() const
{
   return Type;
}

NodeItem* EdgeItem::from() const
{
   return m_from;
}

NodeItem* EdgeItem::to() const
{
   return m_to;
}

const EdgeItem::Info& EdgeItem::info() const
{
   return m_info;
}

void EdgeItem::setParallel(int index, int count)
{
   m_parallelIndex = index;
   m_parallelCount = std::max(1, count);
   updatePath();
}

void EdgeItem::updatePath()
{
   prepareGeometryChange();
   const QPointF c1 = m_from->scenePos();
   const QPointF c2 = m_to->scenePos();
   const qreal offset = (m_parallelIndex - (m_parallelCount - 1) / 2.0) * ParallelSpacing;

   m_path = QPainterPath();
   if (std::abs(offset) < 1e-6)
   {
      m_path.moveTo(m_from->boundaryPoint(c2));
      m_path.lineTo(m_to->boundaryPoint(c1));
   }
   else
   {
      // a quadratic curve reaches half way to its control point
      const QLineF line(c1, c2);
      const QPointF normal = line.length() < 1e-6 ? QPointF(0, -1) : QPointF(-line.dy(), line.dx()) / line.length();
      const QPointF control = (c1 + c2) / 2 + normal * offset * 2;
      m_path.moveTo(m_from->boundaryPoint(control));
      m_path.quadTo(control, m_to->boundaryPoint(control));
   }

   const qreal length = m_path.length();
   if (length > 1e-6)
   {
      const qreal at = std::max(0.55, 1 - LabelDistance / length);
      const qreal angle = qDegreesToRadians(m_path.angleAtPercent(at));
      // to the left of the direction of the line, angles go counterclockwise with y up
      const QPointF side(-std::sin(angle), -std::cos(angle));
      m_cardinalityAt = m_path.pointAtPercent(at) + side * LabelOffset;
      // the role nearer the relationship, clear of the cardinality
      const qreal roleAt = std::min(0.4, std::max(0.25, 40 / length));
      const qreal roleAngle = qDegreesToRadians(m_path.angleAtPercent(roleAt));
      m_roleAt = m_path.pointAtPercent(roleAt) - QPointF(-std::sin(roleAngle), -std::cos(roleAngle)) * LabelOffset;
   }
   else
   {
      m_cardinalityAt = m_roleAt = c2;
   }
   update();
}

void EdgeItem::setHighlighted(bool highlighted)
{
   if (m_highlighted != highlighted)
   {
      m_highlighted = highlighted;
      update();
   }
}

QRectF EdgeItem::labelRect(const QString &text, const QPointF &center) const
{
   const QFontMetricsF metrics(m_style->labelFont);
   const QSizeF size(metrics.horizontalAdvance(text) + 6, metrics.height() + 2);
   return QRectF(center - QPointF(size.width() / 2, size.height() / 2), size);
}

QRectF EdgeItem::boundingRect() const
{
   QRectF rect = m_path.boundingRect().adjusted(-TotalOuterWidth, -TotalOuterWidth, TotalOuterWidth, TotalOuterWidth);
   if (!m_info.cardinality.isEmpty())
   {
      rect = rect.united(labelRect(m_info.cardinality, m_cardinalityAt));
   }
   if (!m_info.role.isEmpty())
   {
      rect = rect.united(labelRect(m_info.role, m_roleAt));
   }
   return rect;
}

QPainterPath EdgeItem::shape() const
{
   QPainterPathStroker stroker;
   stroker.setWidth(10);
   return stroker.createStroke(m_path);
}

void EdgeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
   const Theme& theme = m_style->theme;
   painter->setRenderHint(QPainter::Antialiasing);
   painter->setBrush(Qt::NoBrush);
   const QColor color = m_highlighted && !m_style->exporting ? theme.highlight : theme.line;
   if (m_info.total)
   {
      // a double line: a wide one with the canvas color inside
      painter->setPen(QPen(color, TotalOuterWidth, Qt::SolidLine, Qt::FlatCap));
      painter->drawPath(m_path);
      painter->setPen(QPen(theme.canvas, TotalInnerWidth, Qt::SolidLine, Qt::FlatCap));
      painter->drawPath(m_path);
   }
   else
   {
      painter->setPen(QPen(color, m_highlighted && !m_style->exporting ? LineWidth + 1 : LineWidth));
      painter->drawPath(m_path);
   }

   auto drawLabel = [&](const QString& text, const QPointF& center, const QColor& textColor, bool italic) {
      if (text.isEmpty())
      {
         return;
      }
      QFont font = m_style->labelFont;
      font.setItalic(italic);
      painter->setFont(font);
      const QRectF rect = labelRect(text, center);
      QColor background = theme.canvas;
      background.setAlpha(210);
      painter->setPen(Qt::NoPen);
      painter->setBrush(background);
      painter->drawRoundedRect(rect, 3, 3);
      painter->setPen(textColor);
      painter->drawText(rect, Qt::AlignCenter, text);
   };
   drawLabel(m_info.cardinality, m_cardinalityAt, theme.label, false);
   drawLabel(m_info.role, m_roleAt, theme.nodeText, true);
}

void EdgeItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
   diagramScene()->setHoveredEdge(this);
   QGraphicsItem::hoverEnterEvent(event);
}

void EdgeItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
   diagramScene()->setHoveredEdge(nullptr);
   QGraphicsItem::hoverLeaveEvent(event);
}

void EdgeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
   // accepted, so the release comes here
   event->accept();
}

void EdgeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
   if (event->button() == Qt::LeftButton)
   {
      diagramScene()->activate(m_info.line);
   }
}

DiagramScene* EdgeItem::diagramScene() const
{
   return static_cast<DiagramScene*>(scene());
}
