#include "PositionHolder.h"

PositionHolder::PositionHolder() : m_position(QPointF(0, 0))
{

}

PositionHolder::PositionHolder(const QPointF &position) : m_position(position)
{

}

QPointF PositionHolder::position() const
{
   return m_position;
}

void PositionHolder::setPosition(const QPointF &position)
{
   m_position = position;
}


