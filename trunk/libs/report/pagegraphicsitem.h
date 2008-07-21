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
#ifndef REPORTPAGEGRAPHICSITEM_H
#define REPORTPAGEGRAPHICSITEM_H

#include <QGraphicsItem>

#include "page.h"

#define screen_heightMM (((double)QDesktopWidget().screen()->height() /(double)QDesktopWidget().screen()->physicalDpiY() )*25.4)
#define screen_widthMM (((double)QDesktopWidget().screen()->width() /(double)QDesktopWidget().screen()->physicalDpiX() )*25.4)

namespace Report
{


class PageGraphicsItem : public QGraphicsItem
{
public:
	PageGraphicsItem(Page * page);
	~PageGraphicsItem();
	QRectF boundingRect() const;
	void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

private:
	Page * m_page;
};

}

#endif

