#include "grid.h"
#define DELTAX 20
#define DELTAY 20

Grid * Grid::m_instance = 0;

Grid::Grid()
    :m_snapX(false)
    ,m_snapY(false)
    ,m_deltaX(DELTAX)
    ,m_deltaY(DELTAY)
{
}

Grid * Grid::instance()
{
    if (!m_instance)
	m_instance = new Grid();

    return Grid::m_instance;
}

void Grid::snap(bool s)
{
    snap(s,s);
}

void Grid::snap(bool x, bool y)
{
    m_snapX = x;
    m_snapY = y;
}

qreal Grid::adjustX(qreal x) const
{
    return m_snapX ? (int)(x / m_deltaX) * m_deltaX /*+ 1*/ : x;
}

qreal Grid::adjustY(qreal y) const
{
    return m_snapY ? (int)(y / m_deltaY) * m_deltaY /*+ 1*/ : y;
}

QPointF Grid::adjustPoint(QPointF point) const
{
    return QPointF (m_snapX ? (int)(point.x() / m_deltaX) * m_deltaX /*+ 1*/ : point.x(), m_snapY ? (int)(point.y() / m_deltaY) * m_deltaY /* + 1*/ : point.y());
}

void Grid::setDeltaX(int x)
{
    m_deltaX = x;
}

void Grid::setDeltaY(int y)
{
    m_deltaY = y;
}

void Grid::setDelta(int d)
{
    m_deltaX = m_deltaY = d;
}

int Grid::deltaX()
{
    return m_deltaX;
}

int Grid::deltaY()
{
    return m_deltaY;
}
