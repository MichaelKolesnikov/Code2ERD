#pragma once
#include <QPointF>

class PositionHolder
{
public:
   explicit PositionHolder();
   explicit PositionHolder(const QPointF&);

   QPointF position() const;
   void setPosition(const QPointF&);

private:
   QPointF m_position;
};
