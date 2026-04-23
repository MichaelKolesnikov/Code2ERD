#pragma once
#include <QPointF>

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
   static QPointF projectPointOntoPerpendicular(const QPointF &p1, const QPointF &p2, const QPointF &p, const QPointF &base);
};

