#include "LineGeometryManager.h"
#include "Models/LineModel.h"

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

void LineGeometryManager::updateNode(int node, const QPointF &p)
{
   if (node == 0)
   {

   }
}

void LineGeometryManager::updatePart(int, qreal)
{

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
   return nodes(p1, middle, isFirstPartHorizontal, bendNumber / 2) + nodes(middle, p2, isFirstPartHorizontal ^ ((bendNumber + 1) / 2 % 2 == 1), bendNumber / 2);
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
