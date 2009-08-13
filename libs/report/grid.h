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

#ifndef GRID_H
#define GRID_H

#include <QtCore>

class Grid
{
public:
    Grid();

    static Grid * instance();

    void snap(bool x, bool y);
    void snap(bool s);
    qreal adjustX(qreal x) const;
    qreal adjustY(qreal y) const;
    QPointF adjustPoint(QPointF point) const;

    void setDeltaX(int x);
    void setDeltaY(int y);
    void setDelta(int d);

    int deltaX();
    int deltaY();

private:
    static Grid * m_instance;
    bool m_snapX;
    bool m_snapY;
    int m_deltaX;
    int m_deltaY;
};

#endif // GRID_H
