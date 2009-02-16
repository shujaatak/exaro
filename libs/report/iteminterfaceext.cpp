/***************************************************************************
 *   This file is part of the eXaro project                                *
 *   Copyright (C) 2008 by BogDan Vatra                                    *
 *   bog_dan_ro@yahoo.com                                                  *
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

#include <QGraphicsScene>
#include <QPainter>

#include "iteminterfaceext.h"
#include "pageinterface.h"

#include <limits.h>

namespace Report
{

ItemInterfaceExt::ItemInterfaceExt(QGraphicsItem* parent, QObject * parentObject)
		: ItemInterface(parent, parentObject)
{
    m_BGMode = TransparentMode;
    m_font=QFont("Serif");
    m_font.setPointSizeF(3.5);
    m_font.setStyleStrategy(QFont::PreferMatch);
    m_font.setStyleStrategy(QFont::ForceOutline);
}

ItemInterfaceExt::~ItemInterfaceExt()
{

}

QRectF ItemInterfaceExt::adjustRect(QRectF & rect)
{
	qreal penwidth = pen().widthF();
	rect=rect.adjusted(penwidth,penwidth,-penwidth,-penwidth);
	return rect;
}


QBrush ItemInterfaceExt::brush()
{
	return m_brush;
}

void ItemInterfaceExt::setBrush(const QBrush & brush)
{
	m_brush = brush;
	update();
}

QBrush ItemInterfaceExt::backgroundBrush()
{
	return m_backgroundBrush;
}

void ItemInterfaceExt::setBackgroundBrush(const QBrush & brush)
{
	m_backgroundBrush = brush;
	update();
}


QPen ItemInterfaceExt::pen()
{
	return m_pen;
}

void ItemInterfaceExt::setPen(const QPen & pen)
{
	m_pen = pen;
	update();
}

QFont ItemInterfaceExt::font()
{
	return m_font;
}

void ItemInterfaceExt::setFont(const QFont & font)
{
	m_font = font;
	update();
}

void ItemInterfaceExt::setupPainter(QPainter * painter)
{
    if (!painter)
	return;
    painter->setBrush(brush());
    painter->setPen(pen());
    painter->setBackgroundMode((Qt::BGMode)m_BGMode);
    painter->setBackground(backgroundBrush());
    QFont f=font();
    f.setPixelSize(font().pointSizeF()/UNIT);
    painter->setFont(f);
    painter->setOpacity((qreal)opacity()/100.);
}

ItemInterfaceExt::BGMode ItemInterfaceExt::backgroundMode()
{
	return m_BGMode;
}

void ItemInterfaceExt::setBackgroundMode(BGMode bgMode)
{
	m_BGMode = bgMode;
	update(boundingRect());
}



}
