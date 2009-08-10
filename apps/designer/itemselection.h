#ifndef ITEMSELECTION_H
#define ITEMSELECTION_H

//#include "formeditor_global.h"
//#include <invisible_widget_p.h>
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

#include "iteminterface.h"

#include <QtCore/QHash>
#include <QtCore/QPointer>

class QMouseEvent;
class QPaintEvent;

class ItemSelection;

class ItemHandle: public QGraphicsItem
{
public:
    enum Type
    {
        LeftTop,
        Top,
        RightTop,
        Right,
        RightBottom,
        Bottom,
        LeftBottom,
        Left,

        TypeCount
    };

    ItemHandle(Type t, ItemSelection *s);
    void setItem(Report::ItemInterface *i);
    void setActive(bool a);
    void updateCursor();

    void setEnabled(bool) {}

    QRectF boundingRect () const;

protected:
    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

private:
    void trySetGeometry(Report::ItemInterface *i, int x, int y, int width, int height);
    void tryResize(Report::ItemInterface *i, int width, int height);

private:
    Report::ItemInterface *m_item;
    const Type m_type;
    QPointF m_origPressPos;
    ItemSelection *m_sel;
    QRectF m_geom, m_origGeom;
    bool m_active;
};

class ItemSelection: public QGraphicsItem
{
public:
    ItemSelection();

    void setItem(Report::ItemInterface *i);
    bool isUsed() const;

    void updateActive();
    void updateGeometry();
    void hide();
    void show();
    void update();

    Report::ItemInterface *item() const;

    QRectF boundingRect () const;
protected:
    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

private:
    ItemHandle *m_handles[ItemHandle::TypeCount];
    QPointer<Report::ItemInterface> m_item;
    QRectF m_boundingRect;
};

#endif // ITEMSELECTION_H
