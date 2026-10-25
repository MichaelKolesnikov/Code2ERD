#include "DiagramScene.h"
#include "NodeItem.h"
#include "EdgeItem.h"
#include "RestrictedMenu.h"
#include "Layout/AutoLayout.h"
#include <QPainter>
#include <QUndoStack>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsView>
#include <QHash>
#include <cmath>

namespace
{
   const qreal SceneMargin = 2000;

   // Restores the positions of the shapes before a move or an arrangement
   class LayoutCommand : public QUndoCommand
   {
   public:
      LayoutCommand(DiagramScene* scene, const LayoutDTO& before, const LayoutDTO& after, const QString& text)
         : QUndoCommand(text), m_scene(scene), m_before(before), m_after(after)
      {

      }

      void undo() override
      {
         m_scene->setLayout(m_before);
         emit m_scene->layoutChanged();
      }

      void redo() override
      {
         if (m_first)
         {
            m_first = false; // the scene is already there
            return;
         }
         m_scene->setLayout(m_after);
         emit m_scene->layoutChanged();
      }

   private:
      DiagramScene* m_scene;
      LayoutDTO m_before;
      LayoutDTO m_after;
      bool m_first = true;
   };

   // Moves the positions kept for "from" and its attributes to "to"
   void renameInLayout(QMap<QString, QPointF>& nodes, const QString& from, const QString& to)
   {
      if (from == to || nodes.contains(to) || !nodes.contains(from))
      {
         return;
      }
      const QString prefix = from + '.';
      QMap<QString, QPointF> renamed;
      for (auto it = nodes.begin(); it != nodes.end();)
      {
         if (it.key() == from || it.key().startsWith(prefix))
         {
            renamed.insert(to + it.key().mid(from.size()), it.value());
            it = nodes.erase(it);
         }
         else
         {
            ++it;
         }
      }
      for (auto it = renamed.cbegin(); it != renamed.cend(); ++it)
      {
         nodes.insert(it.key(), it.value());
      }
   }

   // Attributes renamed in place: the same count, a different name at the same index
   void renameAttributes(QMap<QString, QPointF>& nodes, const QString& owner, const QVector<AttributeDTO>& before, const QVector<AttributeDTO>& after)
   {
      if (before.size() != after.size())
      {
         return;
      }
      for (int i = 0; i < before.size(); ++i)
      {
         const QString from = attributeKey(owner, before[i].name);
         const QString to = attributeKey(owner, after[i].name);
         renameInLayout(nodes, from, to);
         renameAttributes(nodes, to, before[i].children, after[i].children);
      }
   }

   template <class T>
   void renameElements(QMap<QString, QPointF>& nodes, const QVector<T>& before, const QVector<T>& after)
   {
      if (before.size() != after.size())
      {
         return;
      }
      for (int i = 0; i < before.size(); ++i)
      {
         renameInLayout(nodes, before[i].name, after[i].name);
         renameAttributes(nodes, after[i].name, before[i].attributes, after[i].attributes);
      }
   }
}

struct DiagramScene::Impl
{
   explicit Impl(DiagramScene* scene) : scene(scene) {}

   void clearItems();
   void buildItems();
   void addAttributes(const QVector<AttributeDTO>& attributes, NodeItem* owner);
   NodeItem* addNode(const NodeInfo&, NodeItem* owner);
   void addEdge(NodeItem* from, NodeItem* to, const EdgeItem::Info&);
   void placeNodes(const QStringList& rearrange = {});
   void storePositions();
   void updateEdges(NodeItem*);
   void updateVisibility();
   void updateSceneRect(bool allowShrink);
   void clearHover();
   void pushLayoutCommand(const QString& text);

   DiagramScene* scene = nullptr;
   DiagramDTO diagram;
   DiagramStyle style;
   LayoutDTO layout;        // positions of everything ever seen, so a shape keeps its place after a typo
   LayoutDTO settledLayout; // after the last finished move: the "before" of the next undo step
   QUndoStack* undoStack = nullptr;
   bool positioning = false; // the scene moves the shapes itself: the attributes do not follow

   QVector<NodeItem*> nodes;
   QHash<QString, NodeItem*> nodeById;
   QVector<EdgeItem*> edges;
   QHash<NodeItem*, QVector<EdgeItem*>> edgesOfNode;
   QHash<NodeItem*, QVector<NodeItem*>> attributesOf;

   QVector<NodeItem*> highlightedNodes;
   QVector<EdgeItem*> highlightedEdges;
};

DiagramScene::DiagramScene(QObject *parent)
   : QGraphicsScene(parent)
{
   pimpl = std::make_unique<Impl>(this);
   pimpl->undoStack = new QUndoStack(this);
   QFont font = QFont();
   font.setPointSizeF(10);
   setDiagramFont(font);
   setSceneRect(-SceneMargin, -SceneMargin, SceneMargin * 2, SceneMargin * 2);
}

DiagramScene::~DiagramScene() = default;

void DiagramScene::Impl::clearItems()
{
   highlightedNodes.clear();
   highlightedEdges.clear();
   for (EdgeItem* edge : qAsConst(edges))
   {
      delete edge;
   }
   for (NodeItem* node : qAsConst(nodes))
   {
      delete node;
   }
   nodes.clear();
   nodeById.clear();
   edges.clear();
   edgesOfNode.clear();
   attributesOf.clear();
}

NodeItem* DiagramScene::Impl::addNode(const NodeInfo &info, NodeItem *owner)
{
   auto node = new NodeItem(info, &style, owner);
   nodes << node;
   if (!nodeById.contains(info.id))
   {
      nodeById.insert(info.id, node);
   }
   if (owner)
   {
      attributesOf[owner] << node;
   }
   return node;
}

void DiagramScene::Impl::addEdge(NodeItem *from, NodeItem *to, const EdgeItem::Info &info)
{
   auto edge = new EdgeItem(from, to, info, &style);
   edges << edge;
   edgesOfNode[from] << edge;
   if (to != from)
   {
      edgesOfNode[to] << edge;
   }
}

void DiagramScene::Impl::addAttributes(const QVector<AttributeDTO> &attributes, NodeItem *owner)
{
   for (const AttributeDTO& attribute : attributes)
   {
      NodeInfo info;
      info.kind = NodeInfo::Kind::Attribute;
      info.id = attributeKey(owner->info().id, attribute.name);
      info.name = attribute.name;
      info.key = attribute.key;
      info.partialKey = attribute.partialKey;
      info.doubleBorder = attribute.multivalued;
      info.dashedBorder = attribute.derived;
      info.note = attribute.note;
      info.line = attribute.line;
      info.endLine = attribute.line;
      NodeItem* node = addNode(info, owner);
      EdgeItem::Info edge;
      edge.line = attribute.line;
      addEdge(owner, node, edge);
      addAttributes(attribute.children, node);
   }
}

void DiagramScene::Impl::buildItems()
{
   for (const EntityDTO& entity : qAsConst(diagram.entities))
   {
      NodeInfo info;
      info.kind = NodeInfo::Kind::Entity;
      info.id = entity.name;
      info.name = entity.name;
      info.doubleBorder = entity.weak;
      info.color = QColor(entity.color);
      info.note = entity.note;
      info.line = entity.line;
      info.endLine = entity.endLine;
      addAttributes(entity.attributes, addNode(info, nullptr));
   }

   for (const RelationshipDTO& relationship : qAsConst(diagram.relationships))
   {
      NodeInfo info;
      info.kind = NodeInfo::Kind::Relationship;
      info.id = relationship.name;
      info.name = relationship.name;
      info.doubleBorder = relationship.identifying;
      info.color = QColor(relationship.color);
      info.note = relationship.note;
      info.line = relationship.line;
      info.endLine = relationship.endLine;
      NodeItem* node = addNode(info, nullptr);

      // lines to the same entity (a recursive relationship) curve apart
      QHash<NodeItem*, int> lineCount;
      for (const ParticipantDTO& participant : relationship.participants)
      {
         NodeItem* entity = nodeById.value(participant.entity);
         if (entity && entity->info().kind == NodeInfo::Kind::Entity)
         {
            ++lineCount[entity];
         }
      }
      QHash<NodeItem*, int> lineIndex;
      for (const ParticipantDTO& participant : relationship.participants)
      {
         NodeItem* entity = nodeById.value(participant.entity);
         if (!entity || entity->info().kind != NodeInfo::Kind::Entity)
         {
            continue;
         }
         EdgeItem::Info edge;
         edge.cardinality = participant.cardinality();
         edge.role = participant.role;
         edge.total = participant.total;
         edge.line = participant.line;
         addEdge(node, entity, edge);
         edges.last()->setParallel(lineIndex[entity]++, lineCount.value(entity));
      }
      addAttributes(relationship.attributes, node);
   }
}

void DiagramScene::Impl::placeNodes(const QStringList& rearrange)
{
   QMap<QString, QSizeF> sizes;
   QMap<QString, QPointF> known;
   for (NodeItem* node : qAsConst(nodes))
   {
      sizes.insert(node->info().id, node->size());
      if (layout.nodes.contains(node->info().id) && !rearrange.contains(node->info().id))
      {
         known.insert(node->info().id, layout.nodes.value(node->info().id));
      }
   }
   const QMap<QString, QPointF> positions = AutoLayout::complete(diagram, sizes, known);
   positioning = true;
   for (NodeItem* node : qAsConst(nodes))
   {
      const QPointF position = positions.value(node->info().id);
      node->setPos(position);
      layout.nodes.insert(node->info().id, position);
   }
   positioning = false;
}

void DiagramScene::Impl::storePositions()
{
   for (NodeItem* node : qAsConst(nodes))
   {
      layout.nodes.insert(node->info().id, node->pos());
   }
}

void DiagramScene::Impl::updateEdges(NodeItem *node)
{
   for (EdgeItem* edge : edgesOfNode.value(node))
   {
      edge->updatePath();
   }
}

void DiagramScene::Impl::updateVisibility()
{
   for (NodeItem* node : qAsConst(nodes))
   {
      node->setVisible(style.showAttributes || node->info().kind != NodeInfo::Kind::Attribute);
   }
   for (EdgeItem* edge : qAsConst(edges))
   {
      edge->setVisible(edge->to()->isVisible());
   }
}

void DiagramScene::Impl::updateSceneRect(bool allowShrink)
{
   const QRectF content = scene->contentRect();
   QRectF wanted = content.isNull() ? QRectF(-SceneMargin, -SceneMargin, SceneMargin * 2, SceneMargin * 2)
                                    : content.adjusted(-SceneMargin, -SceneMargin, SceneMargin, SceneMargin);
   if (!allowShrink)
   {
      wanted = wanted.united(scene->sceneRect());
   }
   if (wanted != scene->sceneRect())
   {
      scene->setSceneRect(wanted);
   }
}

void DiagramScene::Impl::pushLayoutCommand(const QString& text)
{
   storePositions();
   const LayoutDTO after = scene->layout();
   if (after.nodes == settledLayout.nodes)
   {
      return;
   }
   undoStack->push(new LayoutCommand(scene, settledLayout, after, text));
   settledLayout = after;
}

void DiagramScene::setDiagram(const DiagramDTO &diagram)
{
   pimpl->storePositions();
   renameElements(pimpl->layout.nodes, pimpl->diagram.entities, diagram.entities);
   renameElements(pimpl->layout.nodes, pimpl->diagram.relationships, diagram.relationships);
   pimpl->clearItems();
   pimpl->diagram = diagram;
   pimpl->buildItems();
   pimpl->placeNodes();
   for (NodeItem* node : qAsConst(pimpl->nodes))
   {
      addItem(node);
   }
   for (EdgeItem* edge : qAsConst(pimpl->edges))
   {
      addItem(edge);
      edge->updatePath();
   }
   pimpl->updateVisibility();
   pimpl->settledLayout = layout();
   pimpl->updateSceneRect(false);
}

const DiagramDTO& DiagramScene::diagram() const
{
   return pimpl->diagram;
}

bool DiagramScene::hasNodes() const
{
   return !pimpl->nodes.isEmpty();
}

LayoutDTO DiagramScene::layout() const
{
   // only what is on the diagram now
   LayoutDTO result;
   for (NodeItem* node : qAsConst(pimpl->nodes))
   {
      result.nodes.insert(node->info().id, node->pos());
   }
   return result;
}

void DiagramScene::setLayout(const LayoutDTO &layout)
{
   // without items the rebuild has no positions to keep: the given ones win
   pimpl->clearItems();
   pimpl->layout = layout;
   setDiagram(DiagramDTO(pimpl->diagram));
}

void DiagramScene::autoArrange()
{
   pimpl->storePositions();
   QStringList all;
   for (NodeItem* node : qAsConst(pimpl->nodes))
   {
      all << node->info().id;
   }
   pimpl->placeNodes(all);
   for (EdgeItem* edge : qAsConst(pimpl->edges))
   {
      edge->updatePath();
   }
   pimpl->updateSceneRect(true);
   pimpl->pushLayoutCommand(tr("Auto-arrange"));
   emit layoutChanged();
}

const DiagramStyle& DiagramScene::style() const
{
   return pimpl->style;
}

void DiagramScene::setTheme(const Theme &theme)
{
   pimpl->style.theme = theme;
   update();
}

void DiagramScene::setShowAttributes(bool show)
{
   pimpl->style.showAttributes = show;
   pimpl->updateVisibility();
}

void DiagramScene::setSnapToGrid(bool snap)
{
   pimpl->style.snapToGrid = snap;
}

void DiagramScene::setDiagramFont(const QFont &font)
{
   pimpl->style.nodeFont = font;
   pimpl->style.entityFont = font;
   pimpl->style.entityFont.setBold(true);
   pimpl->style.entityFont.setPointSizeF(font.pointSizeF() + 0.5);
   pimpl->style.labelFont = font;
   pimpl->style.labelFont.setBold(true);
   pimpl->style.labelFont.setPointSizeF(std::max(7.0, font.pointSizeF() - 0.5));
   for (NodeItem* node : qAsConst(pimpl->nodes))
   {
      node->relayout();
   }
   for (EdgeItem* edge : qAsConst(pimpl->edges))
   {
      edge->updatePath();
   }
}

void DiagramScene::setExporting(bool exporting)
{
   pimpl->style.exporting = exporting;
   if (exporting)
   {
      pimpl->clearHover();
   }
   update();
}

QUndoStack* DiagramScene::undoStack() const
{
   return pimpl->undoStack;
}

void DiagramScene::selectByLine(int line)
{
   // the deepest thing defined at the line: an attribute or a line of a relationship, else its block
   QGraphicsItem* target = nullptr;
   for (NodeItem* node : qAsConst(pimpl->nodes))
   {
      if (node->isVisible() && node->info().kind == NodeInfo::Kind::Attribute && node->info().line == line)
      {
         target = node;
         break;
      }
   }
   if (!target)
   {
      for (EdgeItem* edge : qAsConst(pimpl->edges))
      {
         if (edge->to()->info().kind == NodeInfo::Kind::Entity && edge->info().line == line)
         {
            target = edge->from();
            pimpl->clearHover();
            edge->setHighlighted(true);
            pimpl->highlightedEdges << edge;
            break;
         }
      }
   }
   if (!target)
   {
      for (NodeItem* node : qAsConst(pimpl->nodes))
      {
         if (node->info().kind != NodeInfo::Kind::Attribute && line >= node->info().line && line <= node->info().endLine)
         {
            target = node;
            break;
         }
      }
   }
   if (target && target->isSelected() && selectedItems().size() == 1)
   {
      return;
   }
   clearSelection();
   if (!target)
   {
      return;
   }
   target->setSelected(true);
   for (QGraphicsView* view : views())
   {
      const QRectF visible = view->mapToScene(view->viewport()->rect()).boundingRect();
      if (!visible.intersects(target->sceneBoundingRect()))
      {
         view->centerOn(target);
      }
   }
}

QRectF DiagramScene::contentRect() const
{
   QRectF bounds;
   auto unite = [&](const QGraphicsItem* item) {
      if (item->isVisible())
      {
         bounds = bounds.isNull() ? item->sceneBoundingRect() : bounds.united(item->sceneBoundingRect());
      }
   };
   for (NodeItem* node : qAsConst(pimpl->nodes))
   {
      unite(node);
   }
   for (EdgeItem* edge : qAsConst(pimpl->edges))
   {
      unite(edge);
   }
   return bounds;
}

NodeItem* DiagramScene::node(const QString &id) const
{
   return pimpl->nodeById.value(id);
}

void DiagramScene::nodeMoved(NodeItem *node, const QPointF &delta)
{
   pimpl->updateEdges(node);
   if (pimpl->positioning)
   {
      return;
   }
   // the attributes go along, unless they are dragged themselves
   for (NodeItem* attribute : pimpl->attributesOf.value(node))
   {
      if (!attribute->isSelected())
      {
         attribute->moveBy(delta.x(), delta.y());
      }
   }
}

void DiagramScene::moveFinished()
{
   pimpl->updateSceneRect(false);
   pimpl->pushLayoutCommand(tr("Move"));
   emit layoutChanged();
}

void DiagramScene::activate(int line)
{
   if (line > 0)
   {
      emit lineActivated(line);
   }
}

void DiagramScene::Impl::clearHover()
{
   for (NodeItem* node : qAsConst(highlightedNodes))
   {
      node->setHighlighted(false);
   }
   for (EdgeItem* edge : qAsConst(highlightedEdges))
   {
      edge->setHighlighted(false);
   }
   highlightedNodes.clear();
   highlightedEdges.clear();
}

void DiagramScene::setHoveredNode(NodeItem *node)
{
   pimpl->clearHover();
   if (!node || pimpl->style.exporting)
   {
      return;
   }
   // the node, its lines and what they lead to: the entities of a relationship, the relationships of an entity
   node->setHighlighted(true);
   pimpl->highlightedNodes << node;
   for (EdgeItem* edge : pimpl->edgesOfNode.value(node))
   {
      if (!edge->isVisible())
      {
         continue;
      }
      edge->setHighlighted(true);
      pimpl->highlightedEdges << edge;
      NodeItem* other = edge->from() == node ? edge->to() : edge->from();
      if (other->info().kind != NodeInfo::Kind::Attribute || node->info().kind == NodeInfo::Kind::Attribute)
      {
         other->setHighlighted(true);
         pimpl->highlightedNodes << other;
      }
   }
}

void DiagramScene::setHoveredEdge(EdgeItem *edge)
{
   pimpl->clearHover();
   if (!edge || pimpl->style.exporting)
   {
      return;
   }
   edge->setHighlighted(true);
   pimpl->highlightedEdges << edge;
   for (NodeItem* node : {edge->from(), edge->to()})
   {
      node->setHighlighted(true);
      pimpl->highlightedNodes << node;
   }
}

void DiagramScene::drawBackground(QPainter *painter, const QRectF &rect)
{
   const Theme& theme = pimpl->style.theme;
   painter->fillRect(rect, theme.canvas);
   if (pimpl->style.exporting)
   {
      return;
   }

   // dots on the grid; fewer of them when zoomed out
   const qreal scale = painter->worldTransform().m11();
   qreal step = pimpl->style.gridSize;
   while (step * scale < 12)
   {
      step *= 2;
   }
   const qreal left = std::floor(rect.left() / step) * step;
   const qreal top = std::floor(rect.top() / step) * step;
   QVector<QPointF> points;
   for (qreal x = left; x < rect.right(); x += step)
   {
      for (qreal y = top; y < rect.bottom(); y += step)
      {
         points << QPointF(x, y);
      }
   }
   QPen pen(theme.gridDot, 2 / std::max(scale, 0.01));
   pen.setCapStyle(Qt::RoundCap);
   painter->setPen(pen);
   painter->drawPoints(points.constData(), points.size());
}

void DiagramScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
   QGraphicsItem* item = itemAt(event->scenePos(), QTransform());
   RestrictedMenu menu;
   if (item && item->type() == NodeItem::Type)
   {
      auto node = static_cast<NodeItem*>(item);
      const int line = node->info().line;
      const QString id = node->info().id;
      menu.addAction(tr("Go to Definition"), this, [this, line]() {emit lineActivated(line);});
      if (!pimpl->attributesOf.value(node).isEmpty())
      {
         menu.addAction(tr("Arrange Attributes"), this, [this, id]() {
            // the attributes of the node, with their parts, go around it again
            pimpl->storePositions();
            QStringList ids;
            for (NodeItem* other : qAsConst(pimpl->nodes))
            {
               if (other->info().id.startsWith(id + '.'))
               {
                  ids << other->info().id;
               }
            }
            pimpl->placeNodes(ids);
            for (EdgeItem* edge : qAsConst(pimpl->edges))
            {
               edge->updatePath();
            }
            pimpl->pushLayoutCommand(tr("Arrange Attributes"));
            emit layoutChanged();
         });
      }
   }
   else
   {
      menu.addAction(tr("Auto-arrange"), this, [this]() {emit autoArrangeRequested();});
      menu.addAction(tr("Fit to Window"), this, [this]() {emit fitRequested();});
   }
   menu.exec(event->screenPos());
}
