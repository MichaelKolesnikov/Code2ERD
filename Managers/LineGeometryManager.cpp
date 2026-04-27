#include "LineGeometryManager.h"
#include "Models/LineModel.h"
#include <QtMath>
#include <QLineF>
#include <QSet>
#include <algorithm>

void LineGeometryManager::set(LineModel *lineModel, const QPointF &p1, const QPointF &p2, bool isFirstPartHorizontal, int bendNumber)
{
   if (bendNumber == 0)
   {
      lineModel->setNodes({p1, p2});
   }
   else if (p1.x() == p2.x() || p1.y() == p2.y())
   {
      lineModel->setNodes({p1, p2});
   }
   else
   {
      lineModel->setNodes(nodes(p1, p2, isFirstPartHorizontal, bendNumber));
   }
}

QVector<QPointF> LineGeometryManager::updateNode(QVector<QPointF> nodes, int node, const QPointF &p, bool free)
{
   if (node >= nodes.size())
   {
      throw "LineGeometryManager::updateNode(node >= nodes.size())";
   }
   if (node < 0)
   {
      throw "LineGeometryManager::updateNode(node < 0)";
   }

   if (free)
   {
      nodes[node] = p;
   }
   else if (nodes.size() == 1)
   {
      nodes.push_back(p);
   }
   else if (node == 0 || node == nodes.size() - 1)
   {
      auto proj = projectPointOntoLine(nodes[node], nodes[node == 0 ? 1 : nodes.size() - 2], p);
      auto proj2 = projectPointOntoPerpendicular(
         nodes[node == 0 ? 0 : nodes.size() - 1],
         nodes[node == 0 ? 1 : nodes.size() - 2],
         p,
         nodes[node == 0 ? 1 : nodes.size() - 2]
      );
      if (dist(proj, nodes[node == 0 ? 1 : nodes.size() - 2]) < delta)
      {
         if (nodes.size() > 2 && dist(nodes[node == 0 ? 2 : nodes.size() - 3], proj2) < delta)
         {
            nodes.remove(node == 0 ? 0 : nodes.size() - 1);
            nodes.remove(node == 0 ? 0 : nodes.size() - 1);
         }
         else if (dist(nodes[node == 0 ? 1 : nodes.size() - 2], proj2) < delta)
         {
            nodes.remove(node == 0 ? 0 : nodes.size() - 1);
         }
         else
         {
            nodes.remove(node == 0 ? 0 : nodes.size() - 1);
            nodes[node == 0 ? 0 : nodes.size() - 1] = proj2;
         }
      }
      else if (dist(proj, p) > delta)
      {
         nodes.insert(node == 0 ? 1 : nodes.size() - 1, proj);
         nodes[node == 0 ? 0 : nodes.size() - 1] = p;
      }
      else
      {
         nodes[node == 0 ? 0 : nodes.size() - 1] = proj;
      }
   }
   else
   {
      int minToRemove = node - 1;
      int maxToRemove = node + 1;
      QVector<QPointF> toInsert;

      auto p_1 = nodes[node - 1];
      auto p0 = nodes[node];
      auto p1 = nodes[node + 1];
      auto centerP = p1 + p_1 - p0;

      std::optional<QPointF> p_2, p2, p_3, p3, p_4, p4;
      auto assign = [&](int offset, std::optional<QPointF>& opt)
      {
         int idx = node + offset;
         if (0 <= idx && idx < nodes.size())
         {
            opt = nodes[idx];
         }
      };
      assign(-2, p_2);
      assign(-3, p_3);
      assign(-4, p_4);
      assign(2, p2);
      assign(3, p3);
      assign(4, p4);

      if (dist(centerP, p) < delta)
      {
         toInsert = {centerP};
      }
      else
      {
         auto newP_1 = intersectLines(p_1, (centerP - p_1), p, (centerP - p1)).value();
         auto newP1 = intersectLines(p1, (centerP - p1), p, (centerP - p_1)).value();


         if (dist(p, newP1) < delta)
         {
            if (p2)
            {
               toInsert = {centerP};
            }
            else
            {
               toInsert = {centerP, newP1};
            }
         }
         else if (dist(p, newP_1) < delta)
         {
            if (p_2)
            {
               toInsert = {centerP};
            }
            else
            {
               toInsert = {newP_1, centerP};
            }
         }
         else
         {
            if (p2 && isOnLine(p, newP1, p2.value(), delta) && p_2 && isOnLine(p, newP_1, p_2.value(), delta))
            {
               toInsert = {projectPointOntoLine(p_2.value(), p_2.value() + (p_1 - p0), p2.value())};
               if (p3)
               {
                  maxToRemove = node + 2;
               }
               if (p_3)
               {
                  minToRemove = node - 2;
               }
            }
            else if (p2 && isOnLine(p, newP1, p2.value(), delta))
            {
               toInsert = {
                  newP_1,
                  projectPointOntoLine(p2.value(), p2.value() + (p - newP1), p)
               };
               if (p3)
               {
                  maxToRemove = node + 2;
                  if (dist(p3.value(), p) < delta)
                  {
                     maxToRemove = node + 3;
                     if (p4)
                     {
                        toInsert = {
                           projectPointOntoLine(
                              p4.value(), p3.value(),
                              p_1
                           )
                        };
                     }
                     else
                     {
                        toInsert = {
                           projectPointOntoLine(
                              p_1, p_1 + (p2.value() - p3.value()),
                              p3.value()
                           ),
                           p3.value(),
                        };
                     }
                  }
               }
            }
            else if (p_2 && isOnLine(p, newP_1, p_2.value(), delta))
            {
               toInsert = {
                  projectPointOntoLine(p_2.value(), p_2.value() + (p - newP_1), p),
                  newP1
               };
               if (p_3)
               {
                  minToRemove = node - 2;
                  if (dist(p_3.value(), p) < delta)
                  {
                     minToRemove = node - 3;
                     if (p_4)
                     {
                        toInsert = {
                           projectPointOntoLine(
                              p_4.value(), p_3.value(),
                              p1
                           )
                        };
                     }
                     else
                     {
                        toInsert = {
                           p_3.value(),
                           projectPointOntoLine(p1, p1 + (p_2.value() - p_3.value()), p_3.value()),
                        };
                     }
                  }
               }
            }
            else
            {
               toInsert = {newP_1, p, newP1};
            }
         }
      }

      nodes.remove(minToRemove, maxToRemove - minToRemove + 1);
      nodes.insert(nodes.begin() + minToRemove, toInsert.size(), {0,0});
      for (int i = 0; i < toInsert.size(); ++i)
      {
         nodes[i + minToRemove] = toInsert[i];
      }
   }

   return nodes;
}

QVector<QPointF> LineGeometryManager::nodes(const QPointF &p1, const QPointF &p2, bool isFirstPartHorizontal, int bendNumber)
{
   if (bendNumber == 0)
   {
      return {p1, p2};
   }
   if (bendNumber == 1)
   {
      return {p1, isFirstPartHorizontal ? QPointF{p2.x(), p1.y()} : QPointF{p1.x(), p2.y()}, p2};
   }
   if (bendNumber == 2)
   {
      if (isFirstPartHorizontal)
      {
         auto x = (p1.x() + p2.x()) / 2;
         return {p1, QPointF(x, p1.y()), QPointF(x, p2.y()), p2};
      }
      else
      {
         auto y = (p1.y() + p2.y()) / 2;
         return {p1, QPointF(p1.x(), y), QPointF(p2.x(), y), p2};
      }
   }
   auto middle = (p1 + p2) / 2;
   auto nodes1 = nodes(p1, middle, isFirstPartHorizontal, bendNumber / 2);
   auto nodes2 = nodes(middle, p2, isFirstPartHorizontal ^ ((bendNumber + 1) / 2 % 2 == 1), bendNumber / 2);
   if (
      nodes1.size() > 0 && nodes2.size() > 0 &&
      (
         nodes1[nodes1.size() - 1].y() == nodes2[0].y()
         ||
         nodes1[nodes1.size() - 1].x() == nodes2[0].x()
      )
   )
   {
      nodes1 = nodes1.mid(0, nodes1.size() - 1);
      if (bendNumber % 2 == 0)
      {
         nodes2 = nodes2.mid(1, nodes2.size() - 1);
      }
   }
   return nodes1 + nodes2;
}

bool LineGeometryManager::isVeryShortToExist(LineModel *lineModel)
{
   if (lineModel->nodes().isEmpty())
   {
      return true;
   }
   qreal length = 0;
   auto& nodes = lineModel->nodes();
   for (int i = 1; i < nodes.size(); ++i)
   {
      length += (nodes[i - 1] - nodes[i]).manhattanLength();
   }
   return length < 20;
}

QPointF LineGeometryManager::projectPointOntoLine(const QPointF &p1, const QPointF &p2, const QPointF &p)
{
   if (p1.x() == p2.x())
   {
      return {p1.x(), p.y()};
   }
   else if (p1.y() == p2.y())
   {
      return {p.x(), p1.y()};
   }

   QPointF v = p2 - p1;
   QPointF w = p - p1;

   auto c1 = QPointF::dotProduct(w, v);
   auto c2 = QPointF::dotProduct(v, v);

   if (c2 < 0.0001)
      return p1;

   auto t = c1 / c2;
   return p1 + t * v;
}

QPointF LineGeometryManager::projectPointOntoPerpendicular(
        const QPointF& p1,
        const QPointF& p2,
        const QPointF& p,
        const QPointF& base)
{
    QPointF v = p2 - p1;
    QPointF v_perp(-v.y(), v.x());

    qreal len2 = v_perp.x() * v_perp.x() + v_perp.y() * v_perp.y();
    if (len2 < 0.0001)
        return p1;

    v_perp = v_perp / qSqrt(len2);

    QPointF w = p - p1;
    qreal t = QPointF::dotProduct(w, v_perp);

    return base + t * v_perp;
}

std::optional<QPointF> LineGeometryManager::intersectLines(const QPointF &a, const QPointF &v1, const QPointF &b, const QPointF &v2)
{
   // a + t * v1 = b + s * v2
   // t * v1 - s * v2 = b - a
   // [v1x  -v2x] [t]   [bx-ax]
   // [v1y  -v2y] [s] = [by-ay]

   QPointF diff = b - a;
   qreal det = v1.x() * (-v2.y()) - (-v2.x()) * v1.y();
   if (std::abs(det) < 1e-8)
      return std::nullopt;

   qreal t = (diff.x() * (-v2.y()) - (-v2.x()) * diff.y()) / det;
   return a + t * v1;
}

bool LineGeometryManager::isOnLine(const QPointF& p1, const QPointF& p2, const QPointF& p, qreal delta)
{
   return dist(projectPointOntoLine(p1, p2, p), p) < delta;
}

qreal LineGeometryManager::dist(const QPointF &p1, const QPointF &p2)
{
   return QLineF(p1, p2).length();
}
