#pragma once
#include <QPointF>
#include <optional>

class LineModel;

class LineGeometryManager
{
public:
   LineGeometryManager() = delete;

   static void set(LineModel*, const QPointF&, const QPointF&, bool isFirstPartHorizontal=true, int bendNumber=2);
   static QVector<QPointF> updateNode(QVector<QPointF>, int, const QPointF&);
   static void updatePart(QVector<QPointF>&, int, qreal);
   static QVector<QPointF> nodes(const QPointF &p1, const QPointF &p2, bool isFirstPartHorizontal=true, int bendNumber=2);
   static bool isVeryShortToExist(LineModel*);
   static QPointF projectPointOntoLine(const QPointF& p1, const QPointF& p2, const QPointF& p);
   static QVector<QPointF> optimizeLine(const QVector<QPointF>&);
   static bool isOnLine(const QPointF& p1, const QPointF& p2, const QPointF& p, qreal delta);
   static qreal dist(const QPointF& p1, const QPointF& p2);

   static inline const qreal delta = 20;
   static inline const qreal eps = 0.01;
   static QPointF projectPointOntoPerpendicular(const QPointF &p1, const QPointF &p2, const QPointF &p, const QPointF &base);

   static std::optional<QPointF> intersectLines(
      const QPointF& a, const QPointF& v1,  // a + t * v1
      const QPointF& b, const QPointF& v2   // b + s * v2
   );
};

