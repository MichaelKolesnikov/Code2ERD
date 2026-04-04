#include "LinesConnectionsModel.h"

LinesConnectionsModel::LinesConnectionsModel()
{

}

void LinesConnectionsModel::set(int lineId, bool in, int objId, const QPointF &pointOnObj)
{
   if (!lineId)
   {
      return;
   }
   QPair<int, QPointF> nullPair = {0, {0,0}};
   if (in && !m_lineInToObj.contains(lineId))
   {
      m_lineInToObj[lineId] = nullPair;
   }
   if (!in && !m_lineOutToObj.contains(lineId))
   {
      m_lineOutToObj[lineId] = nullPair;
   }

   auto curObj = in ? m_lineInToObj[lineId] : m_lineOutToObj[lineId];
   if (curObj != nullPair)
   {
      m_objToLines[curObj.first].remove(lineId);
      if (m_objToLines[curObj.first].size() == 0)
      {
         m_objToLines.remove(curObj.first);
      }
   }

   if (!objId)
   {
      if (in)
      {
         m_lineInToObj.remove(lineId);
         emit lineChanged(lineId, true);
      }
      else
      {
         m_lineOutToObj.remove(lineId);
         emit lineChanged(lineId, false);
      }
   }
   else
   {
      QPair<int, QPointF> p = {objId, pointOnObj};
      if (in)
      {
         if (m_lineInToObj[lineId] != p)
         {
            m_lineInToObj[lineId] = p;
            emit lineChanged(lineId, true, objId, pointOnObj);
         }
      }
      else
      {
         if (m_lineOutToObj[lineId] != p)
         {
            m_lineOutToObj[lineId] = p;
            emit lineChanged(lineId, false, objId, pointOnObj);
         }
      }
      m_objToLines[objId].insert(lineId);
   }
}

void LinesConnectionsModel::removeLine(int lineId)
{
   set(lineId, true);
   set(lineId, false);
}

void LinesConnectionsModel::removeObj(int objId)
{
   if (!objId)
   {
      return;
   }
   auto it = m_objToLines.find(objId);
   if (it != m_objToLines.end()) {
      for (auto lineItem : it.value())
      {
         if (m_lineInToObj.remove(lineItem))
         {
            emit lineChanged(lineItem, true);
         }
         if (m_lineOutToObj.remove(lineItem))
         {
            emit lineChanged(lineItem, false);
         }
      }
   }
   m_objToLines.remove(objId);
}

std::optional<QPointF> LinesConnectionsModel::getAttachmentPoint(int lineId, bool in, int objId) const
{
   if (!lineId || !objId)
   {
      return std::nullopt;
   }

   return
      in
      ?
      (m_lineInToObj.contains(lineId) && m_lineInToObj[lineId].first == objId ? std::optional<QPointF>(m_lineInToObj[lineId].second) : std::nullopt)
      :
      (m_lineOutToObj.contains(lineId) && m_lineOutToObj[lineId].first == objId ? std::optional<QPointF>(m_lineOutToObj[lineId].second) : std::nullopt);
}

int LinesConnectionsModel::getObj(int lineId, bool in) const
{
   if (!lineId)
   {
      return 0;
   }
   if (in)
   {
      if (!m_lineInToObj.contains(lineId))
      {
         return 0;
      }
      return m_lineInToObj[lineId].first;
   }
   else
   {
      if (!m_lineOutToObj.contains(lineId))
      {
         return 0;
      }
      return m_lineOutToObj[lineId].first;
   }
}

QMap<int, QPair<int, QPointF> > LinesConnectionsModel::getInConnections() const
{
   return m_lineInToObj;
}

QMap<int, QPair<int, QPointF> > LinesConnectionsModel::getOutConnections() const
{
   return m_lineOutToObj;
}
