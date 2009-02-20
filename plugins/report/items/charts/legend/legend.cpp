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

#include <QtCore>
#include <QBrush>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "legend.h"

inline void initMyResource()
{
	Q_INIT_RESOURCE(legend);
}

Legend::Legend(QGraphicsItem* parent, QObject* parentObject) : ChartInterface(parent, parentObject), m_legendColorRectWidth(5/UNIT)
{
	initMyResource();
}

QRectF Legend::boundingRect() const
{
	return QRectF(0, 0, width(), height());
}

void Legend::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{
	QRectF rect = (option->type == QStyleOption::SO_GraphicsItem) ? boundingRect() : option->exposedRect;

	if (option->type == QStyleOption::SO_GraphicsItem)
		drawSelection(painter, boundingRect());

	setupPainter(painter);
	adjustRect(rect);

	QList<ChartInterface::_chartValue> val=values();

	if (!val.size())
		return;

	painter->fillRect(rect,brush());
	painter->drawRect(rect);
	painter->translate(rect.topLeft());
	qreal y=1/UNIT;
	qreal vstep=(rect.height()-y-1/UNIT*val.size())/val.size();
	foreach (ChartInterface::_chartValue cv, val)
	{
		painter->fillRect(QRectF(1/UNIT/2,y,m_legendColorRectWidth,vstep),QBrush(cv.color));
		painter->drawText(
		QRectF(1/UNIT+m_legendColorRectWidth,y,rect.width()-(1/UNIT+m_legendColorRectWidth),vstep),Qt::AlignVCenter|Qt::AlignLeft,cv.key);
		y+=vstep+1/UNIT;
	}
}

QIcon Legend::toolBoxIcon()
{
	return QIcon(":/legend.png");
}

QString Legend::toolBoxText()
{
	return tr("Legend");
}

QString Legend::toolBoxGroup()
{
	return tr("Charts");
}

QObject * Legend::createInstance(QGraphicsItem* parent, QObject* parentObject)
{
	return new Legend(parent, parentObject);
}

int Legend::legendColorRectWidth()
{
	return m_legendColorRectWidth;
}

void Legend::setLegendColorRectWidth(int legendColorRectWidth)
{
	if (m_legendColorRectWidth<1)
		legendColorRectWidth=1;
	m_legendColorRectWidth=legendColorRectWidth;
	update();
}

Q_EXPORT_PLUGIN2(legend, Legend)
