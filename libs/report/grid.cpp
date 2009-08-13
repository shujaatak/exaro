/***************************************************************************
 *   This file is part of the eXaro project                                *
 *   Copyright (C) 2009 by Mikhalov Alexaner                               *
 *   alexmi3@rambler.ru                                                    *
 **                   GNU General Public License Usage                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 **                  GNU Lesser General Public License                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library.                                      *
 *   If not, see <http://www.gnu.org/licenses/>.                           *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ****************************************************************************/

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
