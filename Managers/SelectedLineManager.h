#pragma once
#include <QObject>
#include <QPointF>
#include <QList>
#include "ModelItemBinding.h"

template <class T>
class ObjectPool;
class LineItem;
class GhostLineItem;
class AnchorItem;
class Scene;
class QGraphicsSceneMouseEvent;

class SelectedLineManager : public QObject
{
   Q_OBJECT
public:
   explicit SelectedLineManager(Scene* scene, const QMap<QString, Binding>& idToBinding);

   ~SelectedLineManager();

   void selectLineItem(LineItem*);
   void removeLineItemSelection();

   void prepareForAnchorMoving(AnchorItem* currentDraggedAnchor);
   void moveChosenAnchor(QGraphicsSceneMouseEvent *event);
   void finishToMoveChosenAnchor(QGraphicsSceneMouseEvent *event);

   bool hasLine() const;
   bool isStartAnchorChosen() const;
   bool isEndAnchorChosen() const;

   bool m_freeNodesEditing;

private:
   inline static qreal m_snapRadius = 20.0;
   const QMap<QString, Binding>& m_idToBinding;

   Scene* m_diagramScene;
   ObjectPool<AnchorItem>* m_anchorItemObjectPool;

   LineItem* m_selectedLine;
   GhostLineItem* m_tmpLine;
   QList<AnchorItem*> m_anchors;

   QList<QPointF> m_oldPoints;
   AnchorItem* m_draggedAnchor;
   QPointF m_initialDraggedAnchorScenePos;

   void setNewPosToAnchorWithLineNode(QPointF mousePos);
   void setAnchorsOnLineItem(LineItem*);
};
