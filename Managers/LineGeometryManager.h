#pragma once
#include <QPointF>

class LineModel;

class LineGeometryManager
{
public:
   LineGeometryManager() = delete;

   static void set(LineModel*, const QPointF&, const QPointF&, bool isFirstPartHorizontal=true, int bendNumber=2);
   static void updateNode(int, const QPointF&);
   static void updatePart(int, qreal);
   static QVector<QPointF> nodes(const QPointF &p1, const QPointF &p2, bool isFirstPartHorizontal=true, int bendNumber=2);
   static bool isVeryShortToExist(LineModel*);
};

