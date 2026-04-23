#pragma once
#include <QVector>

class QPointF;
class QPainterPath;
class QWidget;
class QStyleOptionGraphicsItem;
class QRectF;
class QPainter;

class LineImpl
{
public:
   LineImpl() = delete;

   static QRectF boundingRect(const QVector<QPointF>&);
   static void paint(const QVector<QPointF>&, QPainter*, const QStyleOptionGraphicsItem*, QWidget *widget = nullptr);
   static QPainterPath shape(const QVector<QPointF>&);

   static QPainterPath painterPath(const QVector<QPointF>&);
};

