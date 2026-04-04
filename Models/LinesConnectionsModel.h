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

   void set(QString lineId, bool in, QString objId="", const QPointF& pointOnObj={0,0});

   void removeLine(QString lineId);

   void removeObj(QString objId);

   std::optional<QPointF> getAttachmentPoint(QString lineId, bool in, QString objId) const;

   QString getObj(QString lineId, bool in) const;

   QMap<QString, QPair<QString, QPointF>> getInConnections() const;
   QMap<QString, QPair<QString, QPointF>> getOutConnections() const;

signals:
   void lineChanged(QString lineId, bool in, QString objId="", const QPointF& pointOnObj={0,0});

private:
   QMap<QString, QPair<QString, QPointF>> m_lineInToObj;
   QMap<QString, QPair<QString, QPointF>> m_lineOutToObj;
   QMap<QString, QSet<QString>> m_objToLines;
};
