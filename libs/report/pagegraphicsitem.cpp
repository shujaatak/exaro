/***************************************************************************
 *   Copyright (C) 2008 by BogDan Vatra                                    *
 *   bogdan@licentia.eu                                                    *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/
#include <QPainter>
#include <QDesktopWidget>
#include <QStyleOptionGraphicsItem>
#include "pagegraphicsitem.h"

namespace Report
{

PageGraphicsItem::PageGraphicsItem(Page * page)
		: QGraphicsItem(0)
{
	setFlags(QGraphicsItem::ItemIsMovable);
	m_page=page;
}

QRectF PageGraphicsItem::boundingRect() const
{
	QRectF rf=QRectF(QPointF(0,0),m_page->pageSize());
	rf.setWidth(rf.width()*(double)QDesktopWidget().screen()->width()/(screen_widthMM*10));
	rf.setHeight(rf.height()*(double)QDesktopWidget().screen()->height()/(screen_heightMM*10));
	return rf;
}

void PageGraphicsItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{
	painter->save();
	painter->fillRect(option->exposedRect,QBrush(QColor(Qt::white)));
	painter->scale(((double)QDesktopWidget().screen()->width()/(screen_widthMM*10)),((double)QDesktopWidget().screen()->height()/(screen_heightMM*10)));
	m_page->render(painter);
	painter->restore();
}

PageGraphicsItem::~PageGraphicsItem()
{
}

}
