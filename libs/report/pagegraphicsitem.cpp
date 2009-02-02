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

#include <QPainter>
#include <QDesktopWidget>
#include <QStyleOptionGraphicsItem>
#include "pagegraphicsitem.h"

namespace Report
{

PageGraphicsItem::PageGraphicsItem(Page * page)
		: QGraphicsItem(0)
{
	m_page=page;
}

QRectF PageGraphicsItem::boundingRect() const
{
	QRectF rf=QRectF(QPointF(0,0),m_page->pageSize());
	return rf;
}

void PageGraphicsItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{
	painter->save();
	painter->fillRect(option->exposedRect,QBrush(QColor(Qt::white)));
	m_page->render(painter, option->exposedRect);
	painter->restore();
}

PageGraphicsItem::~PageGraphicsItem()
{
}

}
