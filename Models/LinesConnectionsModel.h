#pragma once
#include <QMap>
#include <QPointF>
#include <QSet>
#include <QObject>
#include <optional>

class LinesConnectionsModel : public QObject
{
   Q_OBJECT

public:
   LinesConnectionsModel();

   void set(int lineId, bool in, int objId=0, const QPointF& pointOnObj={0,0});

   void removeLine(int lineId);

   void removeObj(int objId);

   std::optional<QPointF> getAttachmentPoint(int lineId, bool in, int objId) const;

   int getObj(int lineId, bool in) const;

   QMap<int, QPair<int, QPointF>> getInConnections() const;
   QMap<int, QPair<int, QPointF>> getOutConnections() const;

signals:
   void lineChanged(int lineId, bool in, int objId=0, const QPointF& pointOnObj={0,0});

private:
   QMap<int, QPair<int, QPointF>> m_lineInToObj;
   QMap<int, QPair<int, QPointF>> m_lineOutToObj;
   QMap<int, QSet<int>> m_objToLines;
};
