#include "AutoLayout.h"
#include "DTO/DiagramDTO.h"
#include <QLineF>
#include <QRectF>
#include <QVector>
#include <algorithm>
#include <cmath>

namespace
{
   const qreal Pi = 3.14159265358979323846;
   const qreal RingGap = 30;      // between an owner and its attributes
   const qreal PartGap = 14;      // between a composite attribute and its parts
   const qreal AttributeSpacing = 10;
   const qreal NodeGap = 60;      // between the footprints of unconnected nodes
   const qreal OverlapMargin = 8;
   const int RelaxIterations = 400;
   const int OverlapIterations = 80;
   const QSizeF DefaultSize(100, 40);

   qreal halfDiagonal(const QSizeF& size)
   {
      return std::hypot(size.width(), size.height()) / 2;
   }

   qreal normalized(qreal angle)
   {
      angle = std::fmod(angle, 2 * Pi);
      return angle < 0 ? angle + 2 * Pi : angle;
   }

   qreal angleOf(const QPointF& from, const QPointF& to)
   {
      return normalized(std::atan2(to.y() - from.y(), to.x() - from.x()));
   }

   QPointF onEllipse(const QPointF& center, qreal rx, qreal ry, qreal angle)
   {
      return center + QPointF(rx * std::cos(angle), ry * std::sin(angle));
   }

   // n directions in the gaps between the blocked ones: a wider gap gets more of them.
   // Without blocked directions they start at the top and go clockwise
   QVector<qreal> freeAngles(QVector<qreal> blocked, int n)
   {
      QVector<qreal> angles;
      if (n <= 0)
      {
         return angles;
      }
      if (blocked.isEmpty())
      {
         for (int i = 0; i < n; ++i)
         {
            angles << normalized(-Pi / 2 + 2 * Pi * i / n);
         }
         return angles;
      }
      for (qreal& angle : blocked)
      {
         angle = normalized(angle);
      }
      std::sort(blocked.begin(), blocked.end());
      QVector<qreal> gaps;
      for (int i = 0; i < blocked.size(); ++i)
      {
         const qreal nextAngle = i + 1 < blocked.size() ? blocked[i + 1] : blocked.first() + 2 * Pi;
         gaps << nextAngle - blocked[i];
      }
      QVector<int> counts(gaps.size(), 0);
      for (int item = 0; item < n; ++item)
      {
         int best = 0;
         for (int i = 1; i < gaps.size(); ++i)
         {
            if (gaps[i] / (counts[i] + 1) > gaps[best] / (counts[best] + 1) + 1e-9)
            {
               best = i;
            }
         }
         ++counts[best];
      }
      for (int i = 0; i < gaps.size(); ++i)
      {
         for (int j = 0; j < counts[i]; ++j)
         {
            angles << normalized(blocked[i] + gaps[i] * (j + 1) / (counts[i] + 1));
         }
      }
      return angles;
   }

   struct Node
   {
      QString key;
      QSizeF size;
      QPointF pos;
      bool known = false;
      bool placed = false;
      bool main = false; // an entity or a relationship
      qreal footprint = 0; // radius of the node with its attributes
      int parent = -1;     // the owner of an attribute
      QVector<int> children;
      QVector<int> neighbors; // relationships <-> entities
   };

   class Layout
   {
   public:
      Layout(const DiagramDTO& diagram, const QMap<QString, QSizeF>& sizes, const QMap<QString, QPointF>& known)
         : m_sizes(sizes), m_known(known)
      {
         for (const EntityDTO& entity : diagram.entities)
         {
            const int index = addNode(entity.name, -1);
            addAttributes(entity.attributes, index);
         }
         for (const RelationshipDTO& relationship : diagram.relationships)
         {
            const int index = addNode(relationship.name, -1);
            addAttributes(relationship.attributes, index);
            for (const ParticipantDTO& participant : relationship.participants)
            {
               const int entity = m_mainByName.value(participant.entity, -1);
               if (entity >= 0 && !m_nodes[index].neighbors.contains(entity))
               {
                  m_nodes[index].neighbors << entity;
                  m_nodes[entity].neighbors << index;
               }
            }
         }
      }

      QMap<QString, QPointF> run()
      {
         for (int i = 0; i < m_nodes.size(); ++i)
         {
            if (m_nodes[i].main)
            {
               m_nodes[i].footprint = footprint(i);
            }
         }
         placeMainNodes();
         for (int i = 0; i < m_nodes.size(); ++i)
         {
            if (m_nodes[i].placed && !m_nodes[i].children.isEmpty())
            {
               placeChildren(i);
            }
         }
         removeOverlaps();

         QMap<QString, QPointF> result;
         for (const Node& node : qAsConst(m_nodes))
         {
            result.insert(node.key, node.pos);
         }
         return result;
      }

   private:
      int addNode(const QString& key, int parent)
      {
         Node node;
         node.key = key;
         node.size = m_sizes.value(key, DefaultSize);
         node.parent = parent;
         node.main = parent < 0;
         if (m_known.contains(key))
         {
            node.pos = m_known.value(key);
            node.known = true;
            node.placed = true;
         }
         const int index = m_nodes.size();
         m_nodes << node;
         if (parent >= 0)
         {
            m_nodes[parent].children << index;
         }
         else if (!m_mainByName.contains(key))
         {
            m_mainByName.insert(key, index);
         }
         return index;
      }

      void addAttributes(const QVector<AttributeDTO>& attributes, int owner)
      {
         for (const AttributeDTO& attribute : attributes)
         {
            const int index = addNode(attributeKey(m_nodes[owner].key, attribute.name), owner);
            addAttributes(attribute.children, index);
         }
      }

      // ---- geometry of the attributes around their owner ----

      QSizeF largestChild(int owner) const
      {
         QSizeF largest(0, 0);
         for (int child : m_nodes[owner].children)
         {
            largest = largest.expandedTo(m_nodes[child].size);
         }
         return largest;
      }

      // The ellipse the attributes of a main node sit on, wide enough for them at the given angles
      void ringRadii(int owner, const QVector<qreal>& angles, qreal& rx, qreal& ry) const
      {
         const QSizeF ownerSize = m_nodes[owner].size;
         const QSizeF child = largestChild(owner);
         rx = ownerSize.width() / 2 + child.width() / 2 + RingGap;
         ry = ownerSize.height() / 2 + child.height() / 2 + RingGap;
         if (angles.size() < 2)
         {
            return;
         }
         QVector<qreal> sorted = angles;
         std::sort(sorted.begin(), sorted.end());
         qreal scale = 1;
         for (int i = 0; i < sorted.size(); ++i)
         {
            const qreal a = sorted[i];
            const qreal b = i + 1 < sorted.size() ? sorted[i + 1] : sorted.first() + 2 * Pi;
            const QPointF p = onEllipse(QPointF(), rx, ry, a);
            const QPointF q = onEllipse(QPointF(), rx, ry, b);
            // neighbors side by side need their width, one above the other their height
            const qreal horizontal = std::abs(std::sin((a + b) / 2));
            const qreal need = horizontal * (child.width() + AttributeSpacing) + (1 - horizontal) * (child.height() + AttributeSpacing);
            const qreal have = QLineF(p, q).length();
            if (have > 1e-6)
            {
               scale = std::max(scale, need / have);
            }
         }
         scale = std::min(scale, 4.0);
         rx *= scale;
         ry *= scale;
      }

      // Parts of a composite attribute: a fan pointing away from where the attribute is attached
      QVector<QPointF> fan(int parent, qreal direction, int count) const
      {
         const QSizeF parentSize = m_nodes[parent].size;
         const QSizeF child = largestChild(parent);
         qreal distance = std::max(parentSize.width(), parentSize.height()) / 2 + child.width() / 2 + PartGap;
         qreal step = (child.height() + AttributeSpacing) / distance;
         // too many parts for a half circle: further out
         if (step * (count - 1) > Pi * 0.9)
         {
            distance *= step * (count - 1) / (Pi * 0.9);
            step = Pi * 0.9 / std::max(1, count - 1);
         }
         QVector<QPointF> points;
         for (int i = 0; i < count; ++i)
         {
            const qreal angle = direction + (i - (count - 1) / 2.0) * step;
            points << QPointF(distance * std::cos(angle), distance * std::sin(angle));
         }
         return points;
      }

      // How far a node with its attributes reaches from its center, when nothing blocks the attributes
      qreal footprint(int owner) const
      {
         const Node& node = m_nodes[owner];
         if (node.children.isEmpty())
         {
            return halfDiagonal(node.size);
         }
         const QVector<qreal> angles = freeAngles({}, node.children.size());
         qreal rx = 0;
         qreal ry = 0;
         ringRadii(owner, angles, rx, ry);
         qreal reach = halfDiagonal(node.size);
         for (int i = 0; i < node.children.size(); ++i)
         {
            const QPointF at = onEllipse(QPointF(), rx, ry, angles[i]);
            reach = std::max(reach, QLineF(QPointF(), at).length() + subtreeReach(node.children[i], angles[i]));
         }
         return reach;
      }

      // How far an attribute with its parts reaches from its center
      qreal subtreeReach(int attribute, qreal direction) const
      {
         const Node& node = m_nodes[attribute];
         qreal reach = halfDiagonal(node.size);
         const QVector<QPointF> points = fan(attribute, direction, node.children.size());
         for (int i = 0; i < node.children.size(); ++i)
         {
            const qreal angle = std::atan2(points[i].y(), points[i].x());
            reach = std::max(reach, QLineF(QPointF(), points[i]).length() + subtreeReach(node.children[i], angle));
         }
         return reach;
      }

      // ---- entities and relationships ----

      QVector<int> mainNodes() const
      {
         QVector<int> nodes;
         for (int i = 0; i < m_nodes.size(); ++i)
         {
            if (m_nodes[i].main)
            {
               nodes << i;
            }
         }
         return nodes;
      }

      // Breadth-first from the best connected node of each group, so connected nodes start close together
      QVector<int> traversalOrder(const QVector<int>& nodes) const
      {
         QVector<int> order;
         QVector<bool> seen(m_nodes.size(), false);
         while (order.size() < nodes.size())
         {
            int start = -1;
            for (int i : nodes)
            {
               if (!seen[i] && (start < 0 || m_nodes[i].neighbors.size() > m_nodes[start].neighbors.size()))
               {
                  start = i;
               }
            }
            QVector<int> queue{start};
            seen[start] = true;
            for (int head = 0; head < queue.size(); ++head)
            {
               order << queue[head];
               for (int neighbor : m_nodes[queue[head]].neighbors)
               {
                  if (!seen[neighbor])
                  {
                     seen[neighbor] = true;
                     queue << neighbor;
                  }
               }
            }
         }
         return order;
      }

      void placeMainNodes()
      {
         const QVector<int> nodes = mainNodes();
         if (nodes.isEmpty())
         {
            return;
         }
         bool anyKnown = false;
         QRectF knownBounds;
         for (int i : nodes)
         {
            if (m_nodes[i].known)
            {
               anyKnown = true;
               const QRectF rect(m_nodes[i].pos - QPointF(m_nodes[i].footprint, m_nodes[i].footprint),
                                 QSizeF(m_nodes[i].footprint * 2, m_nodes[i].footprint * 2));
               knownBounds = knownBounds.isNull() ? rect : knownBounds.united(rect);
            }
         }

         // starting points: a sunflower spiral in the traversal order, or next to the known neighbors
         const QVector<int> order = traversalOrder(nodes);
         qreal averageFootprint = 0;
         for (int i : nodes)
         {
            averageFootprint += m_nodes[i].footprint;
         }
         averageFootprint /= nodes.size();
         int newOnTheRight = 0;
         for (int k = 0; k < order.size(); ++k)
         {
            Node& node = m_nodes[order[k]];
            if (node.known)
            {
               continue;
            }
            const qreal goldenAngle = 2.39996322972865332;
            const QPointF spiral = QPointF(std::cos(k * goldenAngle), std::sin(k * goldenAngle)) * averageFootprint * 1.6 * std::sqrt(qreal(k));
            if (!anyKnown)
            {
               node.pos = spiral;
            }
            else
            {
               QPointF sum;
               int count = 0;
               for (int neighbor : node.neighbors)
               {
                  if (m_nodes[neighbor].placed)
                  {
                     sum += m_nodes[neighbor].pos;
                     ++count;
                  }
               }
               if (count > 0)
               {
                  const qreal angle = k * goldenAngle;
                  node.pos = sum / count + QPointF(std::cos(angle), std::sin(angle)) * (node.footprint + averageFootprint);
               }
               else
               {
                  node.pos = QPointF(knownBounds.right() + node.footprint + NodeGap, knownBounds.top() + node.footprint + newOnTheRight * (node.footprint * 2 + NodeGap));
                  ++newOnTheRight;
               }
            }
            node.placed = true;
         }

         relax(nodes, !anyKnown);
      }

      // Lines pull connected nodes to a comfortable distance, all nodes push away the ones too close
      void relax(const QVector<int>& nodes, bool gravity)
      {
         bool anyFree = false;
         for (int i : nodes)
         {
            anyFree = anyFree || !m_nodes[i].known;
         }
         if (!anyFree)
         {
            return;
         }
         for (int iteration = 0; iteration < RelaxIterations; ++iteration)
         {
            const qreal alpha = 1.0 - 0.95 * iteration / RelaxIterations;
            for (int a = 0; a < nodes.size(); ++a)
            {
               for (int b = a + 1; b < nodes.size(); ++b)
               {
                  Node& p = m_nodes[nodes[a]];
                  Node& q = m_nodes[nodes[b]];
                  if (p.known && q.known)
                  {
                     continue;
                  }
                  QPointF delta = q.pos - p.pos;
                  qreal distance = std::hypot(delta.x(), delta.y());
                  if (distance < 1e-6)
                  {
                     // the same place: apart in a direction that depends only on the indexes
                     const qreal angle = (nodes[a] * 7 + nodes[b] * 13) % 360 * Pi / 180;
                     delta = QPointF(std::cos(angle), std::sin(angle));
                     distance = 1;
                  }
                  const bool connected = p.neighbors.contains(nodes[b]);
                  qreal move = 0;
                  if (connected)
                  {
                     // the attributes keep away from the lines, so the footprints may overlap a bit
                     const qreal wanted = std::max(halfDiagonal(p.size) + halfDiagonal(q.size) + 70, 0.75 * (p.footprint + q.footprint));
                     move = (distance - wanted) * 0.25 * alpha;
                  }
                  else
                  {
                     const qreal wanted = p.footprint + q.footprint + NodeGap;
                     if (distance < wanted)
                     {
                        move = (distance - wanted) * 0.5 * alpha;
                     }
                  }
                  if (std::abs(move) < 1e-9)
                  {
                     continue;
                  }
                  const QPointF direction = delta / distance;
                  const qreal share = p.known || q.known ? 1.0 : 0.5;
                  if (!p.known)
                  {
                     p.pos += direction * move * share;
                  }
                  if (!q.known)
                  {
                     q.pos -= direction * move * share;
                  }
               }
            }
            if (gravity)
            {
               // keeps the unconnected groups together
               QPointF center;
               for (int i : nodes)
               {
                  center += m_nodes[i].pos;
               }
               center /= nodes.size();
               for (int i : nodes)
               {
                  m_nodes[i].pos += (center - m_nodes[i].pos) * 0.01 * alpha;
               }
            }
         }
      }

      // ---- attributes ----

      void placeChildren(int owner)
      {
         const Node& node = m_nodes[owner];
         QVector<int> missing;
         QVector<qreal> blocked;
         for (int child : node.children)
         {
            if (m_nodes[child].known)
            {
               blocked << angleOf(node.pos, m_nodes[child].pos);
            }
            else
            {
               missing << child;
            }
         }
         if (!missing.isEmpty())
         {
            if (node.main)
            {
               placeRing(owner, missing, blocked);
            }
            else
            {
               placeFan(owner, missing);
            }
         }
      }

      void placeRing(int owner, const QVector<int>& missing, QVector<qreal> blocked)
      {
         const Node& node = m_nodes[owner];
         for (int neighbor : node.neighbors)
         {
            if (m_nodes[neighbor].placed)
            {
               blocked << angleOf(node.pos, m_nodes[neighbor].pos);
            }
         }
         const QVector<qreal> angles = freeAngles(blocked, missing.size());
         qreal rx = 0;
         qreal ry = 0;
         QVector<qreal> all = angles;
         if (missing.size() < node.children.size())
         {
            all << blocked; // the known attributes keep their room
         }
         ringRadii(owner, all, rx, ry);
         for (int i = 0; i < missing.size(); ++i)
         {
            Node& child = m_nodes[missing[i]];
            child.pos = onEllipse(node.pos, rx, ry, angles[i]);
            child.placed = true;
         }
      }

      void placeFan(int parent, const QVector<int>& missing)
      {
         const Node& node = m_nodes[parent];
         const QPointF from = m_nodes[node.parent].pos;
         const qreal direction = QLineF(from, node.pos).length() < 1e-6 ? -Pi / 2 : angleOf(from, node.pos);
         const QVector<QPointF> points = fan(parent, direction, missing.size());
         for (int i = 0; i < missing.size(); ++i)
         {
            Node& child = m_nodes[missing[i]];
            child.pos = node.pos + points[i];
            child.placed = true;
         }
      }

      // Pushes apart the shapes that still overlap; the known ones stay, an attribute gives way before a main node
      void removeOverlaps()
      {
         for (int iteration = 0; iteration < OverlapIterations; ++iteration)
         {
            bool moved = false;
            for (int a = 0; a < m_nodes.size(); ++a)
            {
               for (int b = a + 1; b < m_nodes.size(); ++b)
               {
                  Node& p = m_nodes[a];
                  Node& q = m_nodes[b];
                  if (p.known && q.known)
                  {
                     continue;
                  }
                  const qreal overlapX = (p.size.width() + q.size.width()) / 2 + OverlapMargin - std::abs(p.pos.x() - q.pos.x());
                  const qreal overlapY = (p.size.height() + q.size.height()) / 2 + OverlapMargin - std::abs(p.pos.y() - q.pos.y());
                  if (overlapX <= 0 || overlapY <= 0)
                  {
                     continue;
                  }
                  const qreal weightP = p.known ? 0 : p.main ? 1 : 3;
                  const qreal weightQ = q.known ? 0 : q.main ? 1 : 3;
                  const qreal shareP = weightP / (weightP + weightQ);
                  const qreal shareQ = weightQ / (weightP + weightQ);
                  if (overlapX < overlapY)
                  {
                     const qreal sign = p.pos.x() < q.pos.x() || (p.pos.x() == q.pos.x() && a < b) ? -1 : 1;
                     p.pos.rx() += sign * overlapX * shareP;
                     q.pos.rx() -= sign * overlapX * shareQ;
                  }
                  else
                  {
                     const qreal sign = p.pos.y() < q.pos.y() || (p.pos.y() == q.pos.y() && a < b) ? -1 : 1;
                     p.pos.ry() += sign * overlapY * shareP;
                     q.pos.ry() -= sign * overlapY * shareQ;
                  }
                  moved = true;
               }
            }
            if (!moved)
            {
               return;
            }
         }
      }

      const QMap<QString, QSizeF>& m_sizes;
      const QMap<QString, QPointF>& m_known;
      QVector<Node> m_nodes;
      QMap<QString, int> m_mainByName;
   };
}

QMap<QString, QPointF> AutoLayout::arrange(const DiagramDTO &diagram, const QMap<QString, QSizeF> &sizes)
{
   return complete(diagram, sizes, {});
}

QMap<QString, QPointF> AutoLayout::complete(const DiagramDTO &diagram, const QMap<QString, QSizeF> &sizes, const QMap<QString, QPointF> &known)
{
   return Layout(diagram, sizes, known).run();
}
