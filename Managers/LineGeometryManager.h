#pragma once
#include <QPointF>

class LineGeometryManager
{
public:
   LineGeometryManager() = delete;

   static QVector<QPointF> nodes(const QPointF&, const QPointF&, bool isFirstPartHorizontal=true, int bendNumber=2);
   static QVector<QPointF> updateNode(QVector<QPointF>, int, const QPointF&, bool free=false, bool ctrl=false, bool exact=false);
   static bool isVeryShortToExist(const QVector<QPointF>&);
   static QPointF projectPointOntoLine(const QPointF& p1, const QPointF& p2, const QPointF& p);
   static bool isOnLine(const QPointF& p1, const QPointF& p2, const QPointF& p, qreal delta);
   static qreal dist(const QPointF& p1, const QPointF& p2);

   static inline const qreal eps = 0.01;
   static QPointF projectPointOntoPerpendicular(const QPointF &p1, const QPointF &p2, const QPointF &p, const QPointF &base);

   static QPointF intersectLines(
      const QPointF& a, const QPointF& v1,  // a + t * v1
      const QPointF& b, const QPointF& v2   // b + s * v2
   );

private:
   static QVector<QPointF> nodesRec(const QPointF &p1, const QPointF &p2, bool isFirstPartHorizontal=true, int bendNumber=2);
};

